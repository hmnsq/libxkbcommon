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
#تعریف_کړه YYBISON 30802

/* Bison version string.  */
#تعریف_کړه YYBISON_نسخه "3.8.2"

/* Skeleton name.  */
#تعریف_کړه YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#تعریف_کړه YYPURE 1

/* Push parsers.  */
#تعریف_کړه YYPUSH 0

/* Pull parsers.  */
#تعریف_کړه YYPULL 1


/* Substitute the variable and function names.  */
#تعریف_کړه yyparse         _xkbcommon_parse
#تعریف_کړه yylex           _xkbcommon_lex
#تعریف_کړه yyerror         _xkbcommon_error
#تعریف_کړه yydebug         _xkbcommon_debug
#تعریف_کړه yynerrs         _xkbcommon_nerrs

/* First part of user prologue.  */
#کرښه 22 "src/xkbcomp/parser.y"

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "keysym.h"
#ضمن "xkbcomp/xkbcomp-priv.h"
#ضمن "xkbcomp/parser-priv.h"
#ضمن "xkbcomp/ast-build.h"

بنية parser_param {
    بنية xkb_context *ctx;
    بنية scanner *scanner;
    XkbFile *rtrn;
    بنية parser_keymap_config config;
    bool نور_maps;
};

#تعریف_کړه parser_log_with_کوډ(param, level, verbosity, log_پیغام_id, fmt, ...)   \
    scanner_log_with_کوډ((param)->scanner, level, verbosity, log_پیغام_id, fmt,\
                          ##__VA_ARGS__)

#تعریف_کړه parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#تعریف_کړه parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#تعریف_کړه parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ساكن فراغ
_xkbcommon_error(بنية parser_param *param, ثابت حرف *پیغام)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", پیغام);
}

ساكن bool
resolve_keysym(بنية parser_param *param, بنية sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    إذا (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_کلی_No_نښه;
        أرجع true;
    }

    إذا (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_کلی_Void_نښه;
        أرجع true;
    }

    /* xkb_keysym_from_name needs a C string. */
    حرف buf[XKB_KEYSYM_NAME_MAX_اندازه];
    إذا (name.len >= حجم(buf)) {
        أرجع false;
    }
    حافظه_نقل(buf, name.پیل, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    إذا (sym != XKB_کلی_No_نښه) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        أرجع true;
    }

    أرجع false;
}

#تعریف_کړه param_scanner param->scanner

#کرښه 150 "src/xkbcomp/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  وإلا
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  إذا defined __cplusplus
#   إذا 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   وإلا
#    define YY_NULLPTR 0
#   endif
#  وإلا
#   define YY_NULLPTR ((فراغ*)0)
#  endif
# endif

#ضمن "parser.h"
/* Symbol kind.  */
تعداد yysymbol_ډول_t
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
  YYSYMBOL_کلی = 21,                       /* "key"  */
  YYSYMBOL_ALIAS = 22,                     /* "alias"  */
  YYSYMBOL_ډله = 23,                     /* "group"  */
  YYSYMBOL_MODIFIER_MAP = 24,              /* "modifier_map"  */
  YYSYMBOL_INDICATOR = 25,                 /* "indicator"  */
  YYSYMBOL_SHAPE = 26,                     /* "shape"  */
  YYSYMBOL_کلیګانې = 27,                      /* "keys"  */
  YYSYMBOL_لیکه = 28,                       /* "row"  */
  YYSYMBOL_SECTION = 29,                   /* "section"  */
  YYSYMBOL_OVERLAY = 30,                   /* "overlay"  */
  YYSYMBOL_متن = 31,                      /* "text"  */
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
  YYSYMBOL_ALPHANUMERIC_کلیګانې = 61,         /* "alphanumeric_keys"  */
  YYSYMBOL_MODIFIER_کلیګانې = 62,             /* "modifier_keys"  */
  YYSYMBOL_KEYPAD_کلیګانې = 63,               /* "keypad_keys"  */
  YYSYMBOL_FUNCTION_کلیګانې = 64,             /* "function_keys"  */
  YYSYMBOL_ALTERNATE_ډله = 65,           /* "alternate_group"  */
  YYSYMBOL_DEPRECATED = 66,                /* "deprecated"  */
  YYSYMBOL_YYACCEPT = 67,                  /* $accept  */
  YYSYMBOL_XkbFile = 68,                   /* XkbFile  */
  YYSYMBOL_Xkb_مرکب_Map = 69,           /* XkbCompositeMap  */
  YYSYMBOL_Xkb_مرکب_Type = 70,          /* XkbCompositeType  */
  YYSYMBOL_XkbMapConfigList = 71,          /* XkbMapConfigList  */
  YYSYMBOL_XkbMapConfig = 72,              /* XkbMapConfig  */
  YYSYMBOL_FileType = 73,                  /* FileType  */
  YYSYMBOL_OptFlags = 74,                  /* OptFlags  */
  YYSYMBOL_Flags = 75,                     /* Flags  */
  YYSYMBOL_Flag = 76,                      /* Flag  */
  YYSYMBOL_DeclList = 77,                  /* DeclList  */
  YYSYMBOL_Decl = 78,                      /* Decl  */
  YYSYMBOL_VarDecl = 79,                   /* VarDecl  */
  YYSYMBOL_کلی_Name_Decl = 80,               /* KeyNameDecl  */
  YYSYMBOL_کلی_Alias_Decl = 81,              /* KeyAliasDecl  */
  YYSYMBOL_VModDecl = 82,                  /* VModDecl  */
  YYSYMBOL_VModDefList = 83,               /* VModDefList  */
  YYSYMBOL_VModDef = 84,                   /* VModDef  */
  YYSYMBOL_InterpretDecl = 85,             /* InterpretDecl  */
  YYSYMBOL_Interpret_سمون = 86,            /* InterpretMatch  */
  YYSYMBOL_VarDeclList = 87,               /* VarDeclList  */
  YYSYMBOL_کلی_Type_Decl = 88,               /* KeyTypeDecl  */
  YYSYMBOL_SymbolsDecl = 89,               /* SymbolsDecl  */
  YYSYMBOL_OptSymbolsBody = 90,            /* OptSymbolsBody  */
  YYSYMBOL_SymbolsBody = 91,               /* SymbolsBody  */
  YYSYMBOL_SymbolsVarDecl = 92,            /* SymbolsVarDecl  */
  YYSYMBOL_Multi_کلی_Sym_Or_Action_List = 93,   /* MultiKeySymOrActionList  */
  YYSYMBOL_No_نښه_Or_Action_List = 94,      /* NoSymbolOrActionList  */
  YYSYMBOL_ډله_Compat_Decl = 95,           /* GroupCompatDecl  */
  YYSYMBOL_ModMapDecl = 96,                /* ModMapDecl  */
  YYSYMBOL_کلی_Or_کلی_Sym_List = 97,           /* KeyOrKeySymList  */
  YYSYMBOL_کلی_Or_کلی_Sym = 98,               /* KeyOrKeySym  */
  YYSYMBOL_LedMapDecl = 99,                /* LedMapDecl  */
  YYSYMBOL_LedNameDecl = 100,              /* LedNameDecl  */
  YYSYMBOL_UnknownDecl = 101,              /* UnknownDecl  */
  YYSYMBOL_UnknownCompoundStatementDecl = 102, /* UnknownCompoundStatementDecl  */
  YYSYMBOL_ShapeDecl = 103,                /* ShapeDecl  */
  YYSYMBOL_SectionDecl = 104,              /* SectionDecl  */
  YYSYMBOL_SectionBody = 105,              /* SectionBody  */
  YYSYMBOL_Section_Body_توکی = 106,          /* SectionBodyItem  */
  YYSYMBOL_لیکه_Body = 107,                  /* RowBody  */
  YYSYMBOL_لیکه_Body_توکی = 108,              /* RowBodyItem  */
  YYSYMBOL_کلیګانې_2 = 109,                     /*کلیګانې*/
  YYSYMBOL_کلی_2 = 110,                      /*کلی*/
  YYSYMBOL_OverlayDecl = 111,              /* OverlayDecl  */
  YYSYMBOL_Overlay_کلی_List = 112,           /* OverlayKeyList  */
  YYSYMBOL_Overlay_کلی = 113,               /* OverlayKey  */
  YYSYMBOL_OutlineList = 114,              /* OutlineList  */
  YYSYMBOL_Outline_دننه_List = 115,            /* OutlineInList  */
  YYSYMBOL_CoordList = 116,                /* CoordList  */
  YYSYMBOL_Coord = 117,                    /* Coord  */
  YYSYMBOL_DoodadDecl = 118,               /* DoodadDecl  */
  YYSYMBOL_DoodadType = 119,               /* DoodadType  */
  YYSYMBOL_ډګر_Spec = 120,                /* FieldSpec  */
  YYSYMBOL_Element = 121,                  /* Element  */
  YYSYMBOL_Opt_یوځای_ډېرپېښ = 122,             /* OptMergeMode  */
  YYSYMBOL_یوځای_ډېرپېښ = 123,                /* MergeMode  */
  YYSYMBOL_بیان_List = 124,                 /* ExprList  */
  YYSYMBOL_بیان = 125,                     /*بیان*/
  YYSYMBOL_غړی = 126,                     /*غړی*/
  YYSYMBOL_MultiActionList = 127,          /* MultiActionList  */
  YYSYMBOL_ActionList = 128,               /* ActionList  */
  YYSYMBOL_Non_تشه_Actions = 129,          /* NonEmptyActions  */
  YYSYMBOL_Actions = 130,                  /* Actions  */
  YYSYMBOL_Action = 131,                   /* Action  */
  YYSYMBOL_Lhs = 132,                      /* Lhs  */
  YYSYMBOL_OptTerminal = 133,              /* OptTerminal  */
  YYSYMBOL_Terminal = 134,                 /* Terminal  */
  YYSYMBOL_Multi_کلی_Sym_List = 135,          /* MultiKeySymList  */
  YYSYMBOL_کلی_Sym_List = 136,               /* KeySymList  */
  YYSYMBOL_Non_تشه_کلی_Syms = 137,          /* NonEmptyKeySyms  */
  YYSYMBOL_کلی_Syms = 138,                  /* KeySyms  */
  YYSYMBOL_کلی_Sym = 139,                   /* KeySym  */
  YYSYMBOL_کلی_Sym_Lit = 140,                /* KeySymLit  */
  YYSYMBOL_Signed_شمېره = 141,             /* SignedNumber  */
  YYSYMBOL_شمېره = 142,                   /*شمېره*/
  YYSYMBOL_Float = 143,                    /* Float  */
  YYSYMBOL_Integer = 144,                  /* Integer  */
  YYSYMBOL_کلی_کوډ = 145,                  /* KeyCode  */
  YYSYMBOL_Ident = 146,                    /* Ident  */
  YYSYMBOL_String = 147,                   /* String  */
  YYSYMBOL_OptMapName = 148,               /* OptMapName  */
  YYSYMBOL_MapName = 149                   /* MapName  */
};
تعريفنوع تعداد yysymbol_ډول_t yysymbol_ډول_t;




#که_تعریف_شوی لنډ
# undef لنډ
#د_که_پای

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#که_نه_تعریف_شوی __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# إذا defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#د_که_پای

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#که_تعریف_شوی __INT_LEAST8_MAX__
تعريفنوع __INT_LEAST8_TYPE__ yytype_int8;
#وإلاإذا defined YY_STDINT_H
تعريفنوع int_least8_t yytype_int8;
#وإلا
تعريفنوع نښهدار حرف yytype_int8;
#د_که_پای

#که_تعریف_شوی __INT_LEAST16_MAX__
تعريفنوع __INT_LEAST16_TYPE__ yytype_int16;
#وإلاإذا defined YY_STDINT_H
تعريفنوع int_least16_t yytype_int16;
#وإلا
تعريفنوع لنډ yytype_int16;
#د_که_پای

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#که_تعریف_شوی __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#د_که_پای

#إذا defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
تعريفنوع __UINT_LEAST8_TYPE__ yytype_uint8;
#وإلاإذا (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= لوی_عدد)
تعريفنوع uint_least8_t yytype_uint8;
#وإلاإذا !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= لوی_عدد
تعريفنوع بینښه حرف yytype_uint8;
#وإلا
تعريفنوع لنډ yytype_uint8;
#د_که_پای

#إذا defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
تعريفنوع __UINT_LEAST16_TYPE__ yytype_uint16;
#وإلاإذا (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= لوی_عدد)
تعريفنوع uint_least16_t yytype_uint16;
#وإلاإذا !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= لوی_عدد
تعريفنوع بینښه لنډ yytype_uint16;
#وإلا
تعريفنوع صحيح yytype_uint16;
#د_که_پای

#که_نه_تعریف_شوی YYPTRDIFF_T
# إذا defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_اعظمي __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_اعظمي PTRDIFF_MAX
# وإلا
#  define YYPTRDIFF_T اوږد
#  define YYPTRDIFF_اعظمي LONG_MAX
# endif
#د_که_پای

#که_نه_تعریف_شوی YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined اندازه_t
#  define YYSIZE_T اندازه_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T اندازه_t
# وإلا
#  define YYSIZE_T بینښه
# endif
#د_که_پای

#تعریف_کړه YYSIZE_اعظمي                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_اعظمي < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_اعظمي                         \
            : YY_CAST (YYSIZE_T, -1)))

#تعریف_کړه YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, حجم (X))


/* Stored state numbers (used for stacks). */
تعريفنوع yytype_int16 yy_state_t;

/* State numbers in computations.  */
تعريفنوع صحيح yy_state_fast_t;

#که_نه_تعریف_شوی YY_
# إذا defined YYENABLE_NLS && YYENABLE_NLS
#  إذا ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#د_که_پای


#که_نه_تعریف_شوی YY_ATTRIBUTE_خالص
# إذا defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_خالص __attribute__ ((__pure__))
# وإلا
#  define YY_ATTRIBUTE_خالص
# endif
#د_که_پای

#که_نه_تعریف_شوی YY_ATTRIBUTE_ناکارول_شوی
# إذا defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_ناکارول_شوی __attribute__ ((__unused__))
# وإلا
#  define YY_ATTRIBUTE_ناکارول_شوی
# endif
#د_که_پای

/* Suppress unused-variable warnings by "using" E.  */
#إذا ! defined lint || defined __GNUC__
# define YY_USE(E) ((فراغ) (E))
#وإلا
# define YY_USE(E) /*تشه*/
#د_که_پای

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#إذا defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# إذا __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# وإلا
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_پای_ټکی      \
    _Pragma ("GCC diagnostic pop")
#وإلا
# define YY_INITIAL_ارزښت(ارزښت_2) ارزښت_2
#د_که_پای
#که_نه_تعریف_شوی YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_پای_ټکی
#د_که_پای
#که_نه_تعریف_شوی YY_INITIAL_ارزښت
# define YY_INITIAL_ارزښت(ارزښت_2) /* Nothing. */
#د_که_پای

#إذا defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_پای_ټکی            \
    _Pragma ("GCC diagnostic pop")
#د_که_پای
#که_نه_تعریف_شوی YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_پای_ټکی
#د_که_پای


#تعریف_کړه YY_ASSERT(E) ((فراغ) (0 && (E)))

#إذا 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  إذا YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   وإلا
#    define YYSTACK_ALLOC alloca
#    إذا ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
#  define YYSTACK_FREE(Ptr) افعل { /*تشه*/; } طالما (0)
#  ifndef YYSTACK_ALLOC_اعظمي
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_اعظمي 4032 /* reasonable circa 2006 */
#  endif
# وإلا
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_اعظمي
#   define YYSTACK_ALLOC_اعظمي YYSIZE_اعظمي
#  endif
#  إذا (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined حافظه_ورکړه) \
             && (defined YYFREE || defined حرر)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC حافظه_ورکړه
#   إذا ! defined حافظه_ورکړه && ! defined EXIT_SUCCESS
فراغ *حافظه_ورکړه (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE حرر
#   إذا ! defined حرر && ! defined EXIT_SUCCESS
فراغ حرر (فراغ *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#د_که_پای /* 1 */

#إذا (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
اتحاد yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_اعظمي (YYSIZEOF (اتحاد yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_بایټونه(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_اعظمي)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(ډېری_alloc, ډېری_2)                           \
    افعل                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ډېری_alloc, ډېری_2, yysize);                    \
        ډېری_2 = &yyptr->ډېری_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ډېری_2) + YYSTACK_GAP_اعظمي; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    طالما (0)

#د_که_پای

#إذا defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  إذا defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, شمېر_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (شمېر_2)) * حجم (*(Src)))
#  وإلا
#   define YYCOPY(Dst, Src, شمېر_2)              \
      افعل                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          لكل (yyi = 0; yyi < (شمېر_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      طالما (0)
#  endif
# endif
#د_که_پای /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#تعریف_کړه YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#تعریف_کړه YYLAST   945

/* YYNTOKENS -- Number of terminals.  */
#تعریف_کړه YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#تعریف_کړه YYNNTS  83
/* YYNRULES -- Number of rules.  */
#تعریف_کړه YYNRULES  221
/* YYNSTATES -- Number of states.  */
#تعریف_کړه YYNSTATES  386

/* YYMAXUTOK -- Last valid token kind.  */
#تعریف_کړه YYMAXUTOK   257


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#تعریف_کړه YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_ډول_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
ساكن ثابت yytype_int8 yytranslate[] =
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

#إذا YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
ساكن ثابت yytype_int16 yyrline[] =
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
#د_که_پای

/** Accessing symbol of state STATE.  */
#تعریف_کړه YY_ACCESSING_نښه(State) YY_CAST (yysymbol_ډول_t, yystos[State])

#إذا 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
ساكن ثابت حرف *yysymbol_name (yysymbol_ډول_t yysymbol) YY_ATTRIBUTE_ناکارول_شوی;

ساكن ثابت حرف *
yysymbol_name (yysymbol_ډول_t yysymbol)
{
  ساكن ثابت حرف *ثابت yy_sname[] =
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
  أرجع yy_sname[yysymbol];
}
#د_که_پای

#تعریف_کړه YYPACT_NINF (-225)

#تعریف_کړه yypact_ارزښت_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#تعریف_کړه YYTABLE_NINF (-217)

#تعریف_کړه yytable_ارزښت_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
ساكن ثابت yytype_int16 yypact[] =
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
ساكن ثابت yytype_uint8 yydefact[] =
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
ساكن ثابت yytype_int16 yypgoto[] =
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
ساكن ثابت yytype_int16 yydefgoto[] =
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
ساكن ثابت yytype_int16 yytable[] =
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

ساكن ثابت yytype_int16 yycheck[] =
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
ساكن ثابت yytype_uint8 yystos[] =
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
ساكن ثابت yytype_uint8 yyr1[] =
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
ساكن ثابت yytype_int8 yyr2[] =
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


تعداد { YYENOMEM = -2 };

#تعریف_کړه yyerrok         (yyerrstatus = 0)
#تعریف_کړه yyclearin       (yychar = YYEMPTY)

#تعریف_کړه YYACCEPT        اذهب yyacceptlab
#تعریف_کړه YYABORT         اذهب yyabortlab
#تعریف_کړه YYERROR         اذهب yyerrorlab
#تعریف_کړه YYNOMEM         اذهب yyexhaustedlab


#تعریف_کړه YYRECOVERING()  (!!yyerrstatus)

#تعریف_کړه YYBACKUP(Token, ارزښت_2)                                    \
  افعل                                                              \
    إذا (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (ارزښت_2);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        اذهب yybackup;                                            \
      }                                                           \
    وإلا                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  طالما (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#تعریف_کړه YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#إذا YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(دلیلونه_2)                        \
افعل {                                            \
  إذا (yydebug)                                  \
    YYFPRINTF دلیلونه_2;                             \
} طالما (0)




# define YY_نښه_PRINT(Title, ډول_2, ارزښت_2, Location)                    \
افعل {                                                                      \
  إذا (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_نښه_print (stderr,                                            \
                  ډول_2, ارزښت_2, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} طالما (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

ساكن فراغ
yy_نښه_ارزښت_print (FILE *yyo,
                       yysymbol_ډول_t yykind, YYSTYPE ثابت * ثابت yyvaluep, بنية parser_param *param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  إذا (!yyvaluep)
    أرجع;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_پای_ټکی
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

ساكن فراغ
yy_نښه_print (FILE *yyo,
                 yysymbol_ډول_t yykind, YYSTYPE ثابت * ثابت yyvaluep, بنية parser_param *param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_نښه_ارزښت_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

ساكن فراغ
yy_ډېری_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  لكل (; yybottom <= yytop; yybottom++)
    {
      صحيح yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_ډېری_PRINT(Bottom, سر_2)                            \
افعل {                                                            \
  إذا (yydebug)                                                  \
    yy_ډېری_print ((Bottom), (سر_2));                           \
} طالما (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

ساكن فراغ
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 صحيح yyrule, بنية parser_param *param)
{
  صحيح yylno = yyrline[yyrule];
  صحيح yynrhs = yyr2[yyrule];
  صحيح yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  لكل (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_نښه_print (stderr,
                       YY_ACCESSING_نښه (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(قاعده)          \
افعل {                                    \
  إذا (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, قاعده, param); \
} طالما (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
صحيح yydebug;
#وإلا /* !YYDEBUG */
# define YYDPRINTF(دلیلونه_2) ((فراغ) 0)
# define YY_نښه_PRINT(Title, ډول_2, ارزښت_2, Location)
# define YY_ډېری_PRINT(Bottom, سر_2)
# define YY_REDUCE_PRINT(قاعده)
#د_که_پای /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#که_نه_تعریف_شوی YYINITDEPTH
# define YYINITDEPTH 200
#د_که_پای

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#که_نه_تعریف_شوی YYMAXDEPTH
# define YYMAXDEPTH 10000
#د_که_پای


/* Context of a parse error.  */
تعريفنوع بنية
{
  yy_state_t *yyssp;
  yysymbol_ډول_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
ساكن صحيح
yypcontext_expected_نښې (ثابت yypcontext_t *yyctx,
                            yysymbol_ډول_t yyarg[], صحيح yyargn)
{
  /* Actual size of YYARG. */
  صحيح yycount = 0;
  صحيح yyn = yypact[+*yyctx->yyssp];
  إذا (!yypact_ارزښت_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      صحيح yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      صحيح yychecklim = YYLAST - yyn + 1;
      صحيح yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      صحيح yyx;
      لكل (yyx = yyxbegin; yyx < yyxend; ++yyx)
        إذا (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_ارزښت_is_error (yytable[yyx + yyn]))
          {
            إذا (!yyarg)
              ++yycount;
            وإلا إذا (yycount == yyargn)
              أرجع 0;
            وإلا
              yyarg[yycount++] = YY_CAST (yysymbol_ډول_t, yyx);
          }
    }
  إذا (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  أرجع yycount;
}




#که_نه_تعریف_شوی yystrlen
# إذا defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, مزي_اوږدوالی (S)))
# وإلا
/* Return the length of YYSTR.  */
ساكن YYPTRDIFF_T
yystrlen (ثابت حرف *yystr)
{
  YYPTRDIFF_T yylen;
  لكل (yylen = 0; yystr[yylen]; yylen++)
    استمر;
  أرجع yylen;
}
# endif
#د_که_پای

#که_نه_تعریف_شوی yystpcpy
# إذا defined __GLIBC__ && defined _STRING_H && defined GNU_سرچینه
#  define yystpcpy stpcpy
# وإلا
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
ساكن حرف *
yystpcpy (حرف *yydest, ثابت حرف *yysrc)
{
  حرف *yyd = yydest;
  ثابت حرف *yys = yysrc;

  طالما ((*yyd++ = *yys++) != '\0')
    استمر;

  أرجع yyd - 1;
}
# endif
#د_که_پای



ساكن صحيح
yy_syntax_error_arguments (ثابت yypcontext_t *yyctx,
                           yysymbol_ډول_t yyarg[], صحيح yyargn)
{
  /* Actual size of YYARG. */
  صحيح yycount = 0;
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
  إذا (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      صحيح yyn;
      إذا (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_نښې (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      إذا (yyn == YYENOMEM)
        أرجع YYENOMEM;
      وإلا
        yycount += yyn;
    }
  أرجع yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
ساكن صحيح
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, حرف **yymsg,
                ثابت yypcontext_t *yyctx)
{
  تعداد { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  ثابت حرف *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_ډول_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  صحيح yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  إذا (yycount == YYENOMEM)
    أرجع YYENOMEM;

  بدل (yycount)
    {
#تعریف_کړه YYCASE_(N, S)                       \
      حالة N:                               \
        yyformat = S;                       \
        اكسر
    افتراضي: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#تعریف_لغوه YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    صحيح yyi;
    لكل (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        إذا (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_اعظمي)
          yysize = yysize1;
        وإلا
          أرجع YYENOMEM;
      }
  }

  إذا (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      إذا (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_اعظمي))
        *yymsg_alloc = YYSTACK_ALLOC_اعظمي;
      أرجع -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    حرف *yyp = *yymsg;
    صحيح yyi = 0;
    طالما ((*yyp = *yyformat) != '\0')
      إذا (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      وإلا
        {
          ++yyp;
          ++yyformat;
        }
  }
  أرجع 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

ساكن فراغ
yydestruct (ثابت حرف *yymsg,
            yysymbol_ډول_t yykind, YYSTYPE *yyvaluep, بنية parser_param *param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  إذا (!yymsg)
    yymsg = "Deleting";
  YY_نښه_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  بدل (yykind)
    {
    حالة YYSYMBOL_STRING: /* "string literal"  */
#کرښه 259 "src/xkbcomp/parser.y"
            { حرر(((*yyvaluep).str)); }
#کرښه 1640 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_XkbFile: /* XkbFile  */
#کرښه 257 "src/xkbcomp/parser.y"
            { إذا (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#کرښه 1646 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Xkb_مرکب_Map: /* XkbCompositeMap  */
#کرښه 257 "src/xkbcomp/parser.y"
            { إذا (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#کرښه 1652 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_XkbMapConfigList: /* XkbMapConfigList  */
#کرښه 258 "src/xkbcomp/parser.y"
            { FreeXkbFile(((*yyvaluep).fileList).سر); }
#کرښه 1658 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_XkbMapConfig: /* XkbMapConfig  */
#کرښه 257 "src/xkbcomp/parser.y"
            { إذا (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#کرښه 1664 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_DeclList: /* DeclList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).anyList).سر); }
#کرښه 1670 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Decl: /* Decl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).any)); }
#کرښه 1676 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_VarDecl: /* VarDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#کرښه 1682 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_کلی_Name_Decl: /* KeyNameDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).کلی_کوډ)); }
#کرښه 1688 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_کلی_Alias_Decl: /* KeyAliasDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).کلی_Alias)); }
#کرښه 1694 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_VModDecl: /* VModDecl  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).سر); }
#کرښه 1700 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_VModDefList: /* VModDefList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).سر); }
#کرښه 1706 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_VModDef: /* VModDef  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmod)); }
#کرښه 1712 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_InterpretDecl: /* InterpretDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#کرښه 1718 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Interpret_سمون: /* InterpretMatch  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#کرښه 1724 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_VarDeclList: /* VarDeclList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).سر); }
#کرښه 1730 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_کلی_Type_Decl: /* KeyTypeDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).کلی_Type)); }
#کرښه 1736 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_SymbolsDecl: /* SymbolsDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).syms)); }
#کرښه 1742 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_OptSymbolsBody: /* OptSymbolsBody  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).سر); }
#کرښه 1748 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_SymbolsBody: /* SymbolsBody  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).سر); }
#کرښه 1754 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_SymbolsVarDecl: /* SymbolsVarDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#کرښه 1760 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Multi_کلی_Sym_Or_Action_List: /* MultiKeySymOrActionList  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1766 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_ډله_Compat_Decl: /* GroupCompatDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ډله_Compat)); }
#کرښه 1772 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_ModMapDecl: /* ModMapDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).mod_ماسک_2)); }
#کرښه 1778 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_کلی_Or_کلی_Sym_List: /* KeyOrKeySymList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان_List).سر); }
#کرښه 1784 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_کلی_Or_کلی_Sym: /* KeyOrKeySym  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1790 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_LedMapDecl: /* LedMapDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledMap)); }
#کرښه 1796 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_LedNameDecl: /* LedNameDecl  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledName)); }
#کرښه 1802 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_CoordList: /* CoordList  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1808 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Coord: /* Coord  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1814 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_بیان_List: /* ExprList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان_List).سر); }
#کرښه 1820 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_بیان: /*بیان*/
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1826 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_غړی: /*غړی*/
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1832 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_MultiActionList: /* MultiActionList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان_List).سر); }
#کرښه 1838 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_ActionList: /* ActionList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان_List).سر); }
#کرښه 1844 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Non_تشه_Actions: /* NonEmptyActions  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1850 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Actions: /* Actions  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1856 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Action: /* Action  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1862 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Lhs: /* Lhs  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1868 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_OptTerminal: /* OptTerminal  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1874 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Terminal: /* Terminal  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1880 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Multi_کلی_Sym_List: /* MultiKeySymList  */
#کرښه 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان_List).سر); }
#کرښه 1886 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_کلی_Sym_List: /* KeySymList  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1892 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_Non_تشه_کلی_Syms: /* NonEmptyKeySyms  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1898 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_کلی_Syms: /* KeySyms  */
#کرښه 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).بیان)); }
#کرښه 1904 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_OptMapName: /* OptMapName  */
#کرښه 259 "src/xkbcomp/parser.y"
            { حرر(((*yyvaluep).str)); }
#کرښه 1910 "src/xkbcomp/parser.c"
        اكسر;

    حالة YYSYMBOL_MapName: /* MapName  */
#کرښه 259 "src/xkbcomp/parser.y"
            { حرر(((*yyvaluep).str)); }
#کرښه 1916 "src/xkbcomp/parser.c"
        اكسر;

      افتراضي:
        اكسر;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_پای_ټکی
}






/*----------.
| yyparse.  |
`----------*/

صحيح
yyparse (بنية parser_param *param)
{
/* Lookahead token kind.  */
صحيح yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_ارزښت (ساكن YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_ارزښت (= yyval_default);

    /* Number of syntax errors so far.  */
    صحيح yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    صحيح yyerrstatus = 0;

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

  صحيح yyn;
  /* The return value of yyparse.  */
  صحيح yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_ډول_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  حرف yymsgbuf[128];
  حرف *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = حجم yymsgbuf;

#تعریف_کړه YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  صحيح yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  اذهب yysetstate;


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
  YY_IGNORE_USELESS_CAST_پای_ټکی
  YY_ډېری_PRINT (yyss, yyssp);

  إذا (yyss + yystacksize - 1 <= yyssp)
#إذا !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#وإلا
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# إذا defined yyoverflow
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
# وإلا /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      إذا (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      إذا (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        اتحاد yyalloc *yyptr =
          YY_CAST (اتحاد yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_بایټونه (yystacksize))));
        إذا (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        إذا (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (اوږد, yystacksize)));
      YY_IGNORE_USELESS_CAST_پای_ټکی

      إذا (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#د_که_پای /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  إذا (yystate == YYFINAL)
    YYACCEPT;

  اذهب yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  إذا (yypact_ارزښت_is_default (yyn))
    اذهب yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  إذا (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, param_scanner);
    }

  إذا (yychar <= پای_ټکی_د_FILE)
    {
      yychar = پای_ټکی_د_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  وإلا إذا (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      اذهب yyerrlab1;
    }
  وإلا
    {
      yytoken = YYTRANSLATE (yychar);
      YY_نښه_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  إذا (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    اذهب yydefault;
  yyn = yytable[yyn];
  إذا (yyn <= 0)
    {
      إذا (yytable_ارزښت_is_error (yyn))
        اذهب yyerrlab;
      yyn = -yyn;
      اذهب yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  إذا (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_نښه_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_پای_ټکی

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  اذهب yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  إذا (yyn == 0)
    اذهب yyerrlab;
  اذهب yyreduce;


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
  بدل (yyn)
    {
  حالة 2: /* XkbFile: XkbCompositeMap  */
#کرښه 276 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->نور_maps = !!param->rtrn; (فراغ) yynerrs; }
#کرښه 2195 "src/xkbcomp/parser.c"
    اكسر;

  حالة 3: /* XkbFile: XkbMapConfig  */
#کرښه 278 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->نور_maps = !!param->rtrn; YYACCEPT; }
#کرښه 2201 "src/xkbcomp/parser.c"
    اكسر;

  حالة 4: /* XkbFile: "end of file"  */
#کرښه 280 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = عدم; param->نور_maps = false; }
#کرښه 2207 "src/xkbcomp/parser.c"
    اكسر;

  حالة 5: /* XkbCompositeMap: OptFlags XkbCompositeType OptMapName "{" XkbMapConfigList "}" ";"  */
#کرښه 286 "src/xkbcomp/parser.y"
                        { (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (ParseCommon *) (yyvsp[-2].fileList).سر, (yyvsp[-6].mapFlags)); }
#کرښه 2213 "src/xkbcomp/parser.c"
    اكسر;

  حالة 6: /* XkbCompositeType: "xkb_keymap"  */
#کرښه 289 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#کرښه 2219 "src/xkbcomp/parser.c"
    اكسر;

  حالة 7: /* XkbCompositeType: "xkb_semantics"  */
#کرښه 290 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#کرښه 2225 "src/xkbcomp/parser.c"
    اكسر;

  حالة 8: /* XkbCompositeType: "xkb_layout"  */
#کرښه 291 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#کرښه 2231 "src/xkbcomp/parser.c"
    اكسر;

  حالة 9: /* XkbMapConfigList: XkbMapConfigList XkbMapConfig  */
#کرښه 298 "src/xkbcomp/parser.y"
                        {
                            إذا ((yyvsp[0].file)) {
                                إذا ((yyvsp[-1].fileList).سر) {
                                    (yyval.fileList).سر = (yyvsp[-1].fileList).سر;
                                    (yyval.fileList).وروستی->common.بل = &(yyvsp[0].file)->common;
                                    (yyval.fileList).وروستی = (yyvsp[0].file);
                                } وإلا {
                                    (yyval.fileList).سر = (yyval.fileList).وروستی = (yyvsp[0].file);
                                }
                            }
                        }
#کرښه 2247 "src/xkbcomp/parser.c"
    اكسر;

  حالة 10: /* XkbMapConfigList: %empty  */
#کرښه 309 "src/xkbcomp/parser.y"
                        { (yyval.fileList).سر = (yyval.fileList).وروستی = عدم; }
#کرښه 2253 "src/xkbcomp/parser.c"
    اكسر;

  حالة 11: /* XkbMapConfig: OptFlags FileType OptMapName "{" DeclList "}" ";"  */
#کرښه 315 "src/xkbcomp/parser.y"
                        {
                            إذا ((yyvsp[-6].mapFlags) & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            خوندي_map_name((yyvsp[-4].str)));
                            }
                            (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (yyvsp[-2].anyList).سر, (yyvsp[-6].mapFlags));
                        }
#کرښه 2266 "src/xkbcomp/parser.c"
    اكسر;

  حالة 12: /* FileType: "xkb_keycodes"  */
#کرښه 325 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_KEYCODES; }
#کرښه 2272 "src/xkbcomp/parser.c"
    اكسر;

  حالة 13: /* FileType: "xkb_types"  */
#کرښه 326 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_TYPES; }
#کرښه 2278 "src/xkbcomp/parser.c"
    اكسر;

  حالة 14: /* FileType: "xkb_compatibility"  */
#کرښه 327 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_COMPAT; }
#کرښه 2284 "src/xkbcomp/parser.c"
    اكسر;

  حالة 15: /* FileType: "xkb_symbols"  */
#کرښه 328 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_SYMBOLS; }
#کرښه 2290 "src/xkbcomp/parser.c"
    اكسر;

  حالة 16: /* FileType: "xkb_geometry"  */
#کرښه 329 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_GEOMETRY; }
#کرښه 2296 "src/xkbcomp/parser.c"
    اكسر;

  حالة 17: /* OptFlags: Flags  */
#کرښه 332 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#کرښه 2302 "src/xkbcomp/parser.c"
    اكسر;

  حالة 18: /* OptFlags: %empty  */
#کرښه 333 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = 0; }
#کرښه 2308 "src/xkbcomp/parser.c"
    اكسر;

  حالة 19: /* Flags: Flags Flag  */
#کرښه 336 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = ((yyvsp[-1].mapFlags) | (yyvsp[0].mapFlags)); }
#کرښه 2314 "src/xkbcomp/parser.c"
    اكسر;

  حالة 20: /* Flags: Flag  */
#کرښه 337 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#کرښه 2320 "src/xkbcomp/parser.c"
    اكسر;

  حالة 21: /* Flag: "partial"  */
#کرښه 340 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_PARTIAL; }
#کرښه 2326 "src/xkbcomp/parser.c"
    اكسر;

  حالة 22: /* Flag: "default"  */
#کرښه 341 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEFAULT; }
#کرښه 2332 "src/xkbcomp/parser.c"
    اكسر;

  حالة 23: /* Flag: "hidden"  */
#کرښه 342 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_HIDDEN; }
#کرښه 2338 "src/xkbcomp/parser.c"
    اكسر;

  حالة 24: /* Flag: "alphanumeric_keys"  */
#کرښه 343 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_ALPHANUMERIC; }
#کرښه 2344 "src/xkbcomp/parser.c"
    اكسر;

  حالة 25: /* Flag: "modifier_keys"  */
#کرښه 344 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_MODIFIER; }
#کرښه 2350 "src/xkbcomp/parser.c"
    اكسر;

  حالة 26: /* Flag: "keypad_keys"  */
#کرښه 345 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_KEYPAD; }
#کرښه 2356 "src/xkbcomp/parser.c"
    اكسر;

  حالة 27: /* Flag: "function_keys"  */
#کرښه 346 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_FN; }
#کرښه 2362 "src/xkbcomp/parser.c"
    اكسر;

  حالة 28: /* Flag: "alternate_group"  */
#کرښه 347 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_ALTGR; }
#کرښه 2368 "src/xkbcomp/parser.c"
    اكسر;

  حالة 29: /* Flag: "deprecated"  */
#کرښه 348 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEPRECATED; }
#کرښه 2374 "src/xkbcomp/parser.c"
    اكسر;

  حالة 30: /* Flag: "identifier"  */
#کرښه 350 "src/xkbcomp/parser.y"
                        {
                            ثابت bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_کوډ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_اصغري,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (بینښه)(yyvsp[0].sval).len, (yyvsp[0].sval).پیل,
                                (error ? "" : "; ignored")
                            );
                            إذا (error)
                                YYABORT;
                            (yyval.mapFlags) = 0;
                        }
#کرښه 2393 "src/xkbcomp/parser.c"
    اكسر;

  حالة 31: /* DeclList: DeclList Decl  */
#کرښه 367 "src/xkbcomp/parser.y"
                        {
                            إذا ((yyvsp[0].any)) {
                                إذا ((yyvsp[-1].anyList).سر) {
                                    (yyval.anyList).سر = (yyvsp[-1].anyList).سر; (yyvsp[-1].anyList).وروستی->بل = (yyvsp[0].any); (yyval.anyList).وروستی = (yyvsp[0].any);
                                } وإلا {
                                    (yyval.anyList).سر = (yyval.anyList).وروستی = (yyvsp[0].any);
                                }
                            }
                        }
#کرښه 2407 "src/xkbcomp/parser.c"
    اكسر;

  حالة 32: /* DeclList: DeclList OptMergeMode VModDecl  */
#کرښه 382 "src/xkbcomp/parser.y"
                        {
                            لكل (VModDef *vmod = (yyvsp[0].vmodList).سر; vmod; vmod = (VModDef *) vmod->common.بل)
                                vmod->یوځای = (yyvsp[-1].یوځای);
                            إذا ((yyvsp[-2].anyList).سر) {
                                (yyval.anyList).سر = (yyvsp[-2].anyList).سر; (yyvsp[-2].anyList).وروستی->بل = &(yyvsp[0].vmodList).سر->common; (yyval.anyList).وروستی = &(yyvsp[0].vmodList).وروستی->common;
                            } وإلا {
                                (yyval.anyList).سر = &(yyvsp[0].vmodList).سر->common; (yyval.anyList).وروستی = &(yyvsp[0].vmodList).وروستی->common;
                            }
                        }
#کرښه 2421 "src/xkbcomp/parser.c"
    اكسر;

  حالة 33: /* DeclList: %empty  */
#کرښه 391 "src/xkbcomp/parser.y"
                        { (yyval.anyList).سر = (yyval.anyList).وروستی = عدم; }
#کرښه 2427 "src/xkbcomp/parser.c"
    اكسر;

  حالة 34: /* Decl: OptMergeMode VarDecl  */
#کرښه 395 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].var)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].var);
                        }
#کرښه 2436 "src/xkbcomp/parser.c"
    اكسر;

  حالة 35: /* Decl: OptMergeMode InterpretDecl  */
#کرښه 401 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].interp)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].interp);
                        }
#کرښه 2445 "src/xkbcomp/parser.c"
    اكسر;

  حالة 36: /* Decl: OptMergeMode KeyNameDecl  */
#کرښه 406 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].کلی_کوډ)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].کلی_کوډ);
                        }
#کرښه 2454 "src/xkbcomp/parser.c"
    اكسر;

  حالة 37: /* Decl: OptMergeMode KeyAliasDecl  */
#کرښه 411 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].کلی_Alias)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].کلی_Alias);
                        }
#کرښه 2463 "src/xkbcomp/parser.c"
    اكسر;

  حالة 38: /* Decl: OptMergeMode KeyTypeDecl  */
#کرښه 416 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].کلی_Type)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].کلی_Type);
                        }
#کرښه 2472 "src/xkbcomp/parser.c"
    اكسر;

  حالة 39: /* Decl: OptMergeMode SymbolsDecl  */
#کرښه 421 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].syms)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].syms);
                        }
#کرښه 2481 "src/xkbcomp/parser.c"
    اكسر;

  حالة 40: /* Decl: OptMergeMode ModMapDecl  */
#کرښه 426 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].mod_ماسک_2)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].mod_ماسک_2);
                        }
#کرښه 2490 "src/xkbcomp/parser.c"
    اكسر;

  حالة 41: /* Decl: OptMergeMode GroupCompatDecl  */
#کرښه 431 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ډله_Compat)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ډله_Compat);
                        }
#کرښه 2499 "src/xkbcomp/parser.c"
    اكسر;

  حالة 42: /* Decl: OptMergeMode LedMapDecl  */
#کرښه 436 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledMap)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledMap);
                        }
#کرښه 2508 "src/xkbcomp/parser.c"
    اكسر;

  حالة 43: /* Decl: OptMergeMode LedNameDecl  */
#کرښه 441 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledName)->یوځای = (yyvsp[-1].یوځای);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledName);
                        }
#کرښه 2517 "src/xkbcomp/parser.c"
    اكسر;

  حالة 44: /* Decl: OptMergeMode ShapeDecl  */
#کرښه 445 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = عدم; }
#کرښه 2523 "src/xkbcomp/parser.c"
    اكسر;

  حالة 45: /* Decl: OptMergeMode SectionDecl  */
#کرښه 446 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = عدم; }
#کرښه 2529 "src/xkbcomp/parser.c"
    اكسر;

  حالة 46: /* Decl: OptMergeMode DoodadDecl  */
#کرښه 447 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = عدم; }
#کرښه 2535 "src/xkbcomp/parser.c"
    اكسر;

  حالة 47: /* Decl: OptMergeMode UnknownDecl  */
#کرښه 449 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#کرښه 2541 "src/xkbcomp/parser.c"
    اكسر;

  حالة 48: /* Decl: OptMergeMode UnknownCompoundStatementDecl  */
#کرښه 451 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#کرښه 2547 "src/xkbcomp/parser.c"
    اكسر;

  حالة 49: /* Decl: MergeMode "string literal"  */
#کرښه 453 "src/xkbcomp/parser.y"
                        {
                            (yyval.any) = (ParseCommon *) IncludeCreate(param->ctx, (yyvsp[0].str), (yyvsp[-1].یوځای));
                            حرر((yyvsp[0].str));
                        }
#کرښه 2556 "src/xkbcomp/parser.c"
    اكسر;

  حالة 50: /* VarDecl: Lhs "=" Expr ";"  */
#کرښه 460 "src/xkbcomp/parser.y"
                        { (yyval.var) = VarCreate((yyvsp[-3].بیان), (yyvsp[-1].بیان)); }
#کرښه 2562 "src/xkbcomp/parser.c"
    اكسر;

  حالة 51: /* VarDecl: Ident ";"  */
#کرښه 462 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), true); }
#کرښه 2568 "src/xkbcomp/parser.c"
    اكسر;

  حالة 52: /* VarDecl: "!" Ident ";"  */
#کرښه 464 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), false); }
#کرښه 2574 "src/xkbcomp/parser.c"
    اكسر;

  حالة 53: /* KeyNameDecl: "key name" "=" KeyCode ";"  */
#کرښه 468 "src/xkbcomp/parser.y"
                        { (yyval.کلی_کوډ) = KeycodeCreate((yyvsp[-3].atom), (yyvsp[-1].عدد)); }
#کرښه 2580 "src/xkbcomp/parser.c"
    اكسر;

  حالة 54: /* KeyAliasDecl: "alias" "key name" "=" "key name" ";"  */
#کرښه 472 "src/xkbcomp/parser.y"
                        { (yyval.کلی_Alias) = کلی_Alias_Create((yyvsp[-3].atom), (yyvsp[-1].atom)); }
#کرښه 2586 "src/xkbcomp/parser.c"
    اكسر;

  حالة 55: /* VModDecl: "virtual_modifiers" VModDefList ";"  */
#کرښه 476 "src/xkbcomp/parser.y"
                        { (yyval.vmodList) = (yyvsp[-1].vmodList); }
#کرښه 2592 "src/xkbcomp/parser.c"
    اكسر;

  حالة 56: /* VModDefList: VModDefList "," VModDef  */
#کرښه 480 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).سر = (yyvsp[-2].vmodList).سر; (yyval.vmodList).وروستی->common.بل = &(yyvsp[0].vmod)->common; (yyval.vmodList).وروستی = (yyvsp[0].vmod); }
#کرښه 2598 "src/xkbcomp/parser.c"
    اكسر;

  حالة 57: /* VModDefList: VModDef  */
#کرښه 482 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).سر = (yyval.vmodList).وروستی = (yyvsp[0].vmod); }
#کرښه 2604 "src/xkbcomp/parser.c"
    اكسر;

  حالة 58: /* VModDef: Ident  */
#کرښه 486 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[0].atom), عدم); }
#کرښه 2610 "src/xkbcomp/parser.c"
    اكسر;

  حالة 59: /* VModDef: Ident "=" Expr  */
#کرښه 488 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[-2].atom), (yyvsp[0].بیان)); }
#کرښه 2616 "src/xkbcomp/parser.c"
    اكسر;

  حالة 60: /* InterpretDecl: "interpret" InterpretMatch "{" VarDeclList "}" ";"  */
#کرښه 494 "src/xkbcomp/parser.y"
                        { (yyvsp[-4].interp)->def = (yyvsp[-2].varList).سر; (yyval.interp) = (yyvsp[-4].interp); }
#کرښه 2622 "src/xkbcomp/parser.c"
    اكسر;

  حالة 61: /* InterpretMatch: KeySym "+" Expr  */
#کرښه 498 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[-2].keysym), (yyvsp[0].بیان)); }
#کرښه 2628 "src/xkbcomp/parser.c"
    اكسر;

  حالة 62: /* InterpretMatch: KeySym  */
#کرښه 500 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[0].keysym), عدم); }
#کرښه 2634 "src/xkbcomp/parser.c"
    اكسر;

  حالة 63: /* VarDeclList: VarDeclList VarDecl  */
#کرښه 504 "src/xkbcomp/parser.y"
                        {
                            إذا ((yyvsp[0].var)) {
                                إذا ((yyvsp[-1].varList).سر) {
                                    (yyval.varList).سر = (yyvsp[-1].varList).سر;
                                    (yyval.varList).وروستی->common.بل = &(yyvsp[0].var)->common;
                                    (yyval.varList).وروستی = (yyvsp[0].var);
                                } وإلا {
                                    (yyval.varList).سر = (yyval.varList).وروستی = (yyvsp[0].var);
                                }
                            }
                        }
#کرښه 2650 "src/xkbcomp/parser.c"
    اكسر;

  حالة 64: /* VarDeclList: %empty  */
#کرښه 515 "src/xkbcomp/parser.y"
                        { (yyval.varList).سر = (yyval.varList).وروستی = عدم; }
#کرښه 2656 "src/xkbcomp/parser.c"
    اكسر;

  حالة 65: /* KeyTypeDecl: "type" String "{" VarDeclList "}" ";"  */
#کرښه 521 "src/xkbcomp/parser.y"
                        { (yyval.کلی_Type) = کلی_Type_Create((yyvsp[-4].atom), (yyvsp[-2].varList).سر); }
#کرښه 2662 "src/xkbcomp/parser.c"
    اكسر;

  حالة 66: /* SymbolsDecl: "key" "key name" "{" OptSymbolsBody "}" ";"  */
#کرښه 527 "src/xkbcomp/parser.y"
                        { (yyval.syms) = SymbolsCreate((yyvsp[-4].atom), (yyvsp[-2].varList).سر); }
#کرښه 2668 "src/xkbcomp/parser.c"
    اكسر;

  حالة 67: /* OptSymbolsBody: SymbolsBody  */
#کرښه 530 "src/xkbcomp/parser.y"
                                    { (yyval.varList) = (yyvsp[0].varList); }
#کرښه 2674 "src/xkbcomp/parser.c"
    اكسر;

  حالة 68: /* OptSymbolsBody: %empty  */
#کرښه 531 "src/xkbcomp/parser.y"
                                    { (yyval.varList).سر = (yyval.varList).وروستی = عدم; }
#کرښه 2680 "src/xkbcomp/parser.c"
    اكسر;

  حالة 69: /* SymbolsBody: SymbolsBody "," SymbolsVarDecl  */
#کرښه 535 "src/xkbcomp/parser.y"
                        { (yyval.varList).سر = (yyvsp[-2].varList).سر; (yyval.varList).وروستی->common.بل = &(yyvsp[0].var)->common; (yyval.varList).وروستی = (yyvsp[0].var); }
#کرښه 2686 "src/xkbcomp/parser.c"
    اكسر;

  حالة 70: /* SymbolsBody: SymbolsVarDecl  */
#کرښه 537 "src/xkbcomp/parser.y"
                        { (yyval.varList).سر = (yyval.varList).وروستی = (yyvsp[0].var); }
#کرښه 2692 "src/xkbcomp/parser.c"
    اكسر;

  حالة 71: /* SymbolsVarDecl: Lhs "=" Expr  */
#کرښه 540 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate((yyvsp[-2].بیان), (yyvsp[0].بیان)); }
#کرښه 2698 "src/xkbcomp/parser.c"
    اكسر;

  حالة 72: /* SymbolsVarDecl: Lhs "=" MultiKeySymOrActionList  */
#کرښه 541 "src/xkbcomp/parser.y"
                                                           { (yyval.var) = VarCreate((yyvsp[-2].بیان), (yyvsp[0].بیان)); }
#کرښه 2704 "src/xkbcomp/parser.c"
    اكسر;

  حالة 73: /* SymbolsVarDecl: Ident  */
#کرښه 542 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), true); }
#کرښه 2710 "src/xkbcomp/parser.c"
    اكسر;

  حالة 74: /* SymbolsVarDecl: "!" Ident  */
#کرښه 543 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), false); }
#کرښه 2716 "src/xkbcomp/parser.c"
    اكسر;

  حالة 75: /* SymbolsVarDecl: MultiKeySymOrActionList  */
#کرښه 544 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate(عدم, (yyvsp[0].بیان)); }
#کرښه 2722 "src/xkbcomp/parser.c"
    اكسر;

  حالة 76: /* MultiKeySymOrActionList: "[" MultiKeySymList "]"  */
#کرښه 560 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[-1].بیان_List).سر; }
#کرښه 2728 "src/xkbcomp/parser.c"
    اكسر;

  حالة 77: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiKeySymList "]"  */
#کرښه 562 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoSymbol */
                            بنية {بیان_Def *سر; بیان_Def *وروستی;} list = {
                                .سر = (yyvsp[-1].بیان_List).سر, .وروستی = (yyvsp[-1].بیان_List).وروستی
                            };
                            لكل (uint32_t k = 0; k < (yyvsp[-3].no_نښه_Or_Action_List); k++) {
                                بیان_Def* ثابت syms =
                                    بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه);
                                إذا (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.بل = &list.سر->common;
                                list.سر = syms;
                            }
                            (yyval.بیان) = list.سر;
                        }
#کرښه 2750 "src/xkbcomp/parser.c"
    اكسر;

  حالة 78: /* MultiKeySymOrActionList: "[" MultiActionList "]"  */
#کرښه 580 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[-1].بیان_List).سر; }
#کرښه 2756 "src/xkbcomp/parser.c"
    اكسر;

  حالة 79: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiActionList "]"  */
#کرښه 582 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoAction() */
                            بنية {بیان_Def *سر; بیان_Def *وروستی;} list = {
                                .سر = (yyvsp[-1].بیان_List).سر, .وروستی = (yyvsp[-1].بیان_List).وروستی
                            };
                            لكل (uint32_t k = 0; k < (yyvsp[-3].no_نښه_Or_Action_List); k++) {
                                بیان_Def* ثابت acts = بیان_Create_Action_List(عدم);
                                إذا (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.بل = &list.سر->common;
                                list.سر = acts;
                            }
                            (yyval.بیان) = list.سر;
                        }
#کرښه 2777 "src/xkbcomp/parser.c"
    اكسر;

  حالة 80: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "]"  */
#کرښه 604 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_تشه_List(); }
#کرښه 2783 "src/xkbcomp/parser.c"
    اكسر;

  حالة 81: /* NoSymbolOrActionList: NoSymbolOrActionList "," "{" "}"  */
#کرښه 610 "src/xkbcomp/parser.y"
                        { (yyval.no_نښه_Or_Action_List) = (yyvsp[-3].no_نښه_Or_Action_List) + 1; }
#کرښه 2789 "src/xkbcomp/parser.c"
    اكسر;

  حالة 82: /* NoSymbolOrActionList: "{" "}"  */
#کرښه 612 "src/xkbcomp/parser.y"
                        { (yyval.no_نښه_Or_Action_List) = 1; }
#کرښه 2795 "src/xkbcomp/parser.c"
    اكسر;

  حالة 83: /* NoSymbolOrActionList: %empty  */
#کرښه 613 "src/xkbcomp/parser.y"
                        { (yyval.no_نښه_Or_Action_List) = 0; }
#کرښه 2801 "src/xkbcomp/parser.c"
    اكسر;

  حالة 84: /* GroupCompatDecl: "group" Integer "=" Expr ";"  */
#کرښه 617 "src/xkbcomp/parser.y"
                        { (yyval.ډله_Compat) = ډله_Compat_Create((yyvsp[-3].عدد), (yyvsp[-1].بیان)); }
#کرښه 2807 "src/xkbcomp/parser.c"
    اكسر;

  حالة 85: /* ModMapDecl: "modifier_map" Expr "{" KeyOrKeySymList "}" ";"  */
#کرښه 621 "src/xkbcomp/parser.y"
                        {
                            إذا (param->config.format == XKB_KEYMAP_FORMAT_متن_V1 &&
                                (yyvsp[-4].بیان)->common.type != STMT_بیان_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ماسک,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) (yyvsp[-4].بیان));
                                    FreeStmt((ParseCommon *) (yyvsp[-2].بیان_List).سر);
                                    YYERROR;
                            }
                            (yyval.mod_ماسک_2) = ModMapCreate((yyvsp[-4].بیان), (yyvsp[-2].بیان_List).سر);
                        }
#کرښه 2826 "src/xkbcomp/parser.c"
    اكسر;

  حالة 86: /* KeyOrKeySymList: KeyOrKeySymList "," KeyOrKeySym  */
#کرښه 638 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyvsp[-2].بیان_List).سر; (yyval.بیان_List).وروستی->common.بل = &(yyvsp[0].بیان)->common; (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 2832 "src/xkbcomp/parser.c"
    اكسر;

  حالة 87: /* KeyOrKeySymList: KeyOrKeySym  */
#کرښه 640 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 2838 "src/xkbcomp/parser.c"
    اكسر;

  حالة 88: /* KeyOrKeySym: "key name"  */
#کرښه 644 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_کلی_Name((yyvsp[0].atom)); }
#کرښه 2844 "src/xkbcomp/parser.c"
    اكسر;

  حالة 89: /* KeyOrKeySym: KeySym  */
#کرښه 646 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_کلی_Sym((yyvsp[0].keysym)); }
#کرښه 2850 "src/xkbcomp/parser.c"
    اكسر;

  حالة 90: /* LedMapDecl: "indicator" String "{" VarDeclList "}" ";"  */
#کرښه 650 "src/xkbcomp/parser.y"
                        { (yyval.ledMap) = LedMapCreate((yyvsp[-4].atom), (yyvsp[-2].varList).سر); }
#کرښه 2856 "src/xkbcomp/parser.c"
    اكسر;

  حالة 91: /* LedNameDecl: "indicator" Integer "=" Expr ";"  */
#کرښه 654 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].عدد), (yyvsp[-1].بیان), false); }
#کرښه 2862 "src/xkbcomp/parser.c"
    اكسر;

  حالة 92: /* LedNameDecl: "virtual" "indicator" Integer "=" Expr ";"  */
#کرښه 656 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].عدد), (yyvsp[-1].بیان), true); }
#کرښه 2868 "src/xkbcomp/parser.c"
    اكسر;

  حالة 93: /* UnknownDecl: "identifier" Terminal "=" Expr ";"  */
#کرښه 660 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-3].بیان));
                            FreeStmt((ParseCommon *) (yyvsp[-1].بیان));
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, (yyvsp[-4].sval));
                        }
#کرښه 2878 "src/xkbcomp/parser.c"
    اكسر;

  حالة 94: /* UnknownCompoundStatementDecl: "identifier" OptTerminal "{" VarDeclList "}" ";"  */
#کرښه 669 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-4].بیان));
                            FreeStmt((ParseCommon *) (yyvsp[-2].varList).سر);
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, (yyvsp[-5].sval));
                        }
#کرښه 2888 "src/xkbcomp/parser.c"
    اكسر;

  حالة 95: /* ShapeDecl: "shape" String "{" OutlineList "}" ";"  */
#کرښه 677 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 2894 "src/xkbcomp/parser.c"
    اكسر;

  حالة 96: /* ShapeDecl: "shape" String "{" CoordList "}" ";"  */
#کرښه 679 "src/xkbcomp/parser.y"
                        { (فراغ) (yyvsp[-2].بیان); (yyval.geom) = عدم; }
#کرښه 2900 "src/xkbcomp/parser.c"
    اكسر;

  حالة 97: /* SectionDecl: "section" String "{" SectionBody "}" ";"  */
#کرښه 683 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 2906 "src/xkbcomp/parser.c"
    اكسر;

  حالة 98: /* SectionBody: SectionBody SectionBodyItem  */
#کرښه 686 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = عدم;}
#کرښه 2912 "src/xkbcomp/parser.c"
    اكسر;

  حالة 99: /* SectionBody: SectionBodyItem  */
#کرښه 687 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = عدم; }
#کرښه 2918 "src/xkbcomp/parser.c"
    اكسر;

  حالة 100: /* SectionBodyItem: "row" "{" RowBody "}" ";"  */
#کرښه 691 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 2924 "src/xkbcomp/parser.c"
    اكسر;

  حالة 101: /* SectionBodyItem: VarDecl  */
#کرښه 693 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = عدم; }
#کرښه 2930 "src/xkbcomp/parser.c"
    اكسر;

  حالة 102: /* SectionBodyItem: DoodadDecl  */
#کرښه 695 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 2936 "src/xkbcomp/parser.c"
    اكسر;

  حالة 103: /* SectionBodyItem: LedMapDecl  */
#کرښه 697 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ledMap)); (yyval.geom) = عدم; }
#کرښه 2942 "src/xkbcomp/parser.c"
    اكسر;

  حالة 104: /* SectionBodyItem: OverlayDecl  */
#کرښه 699 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 2948 "src/xkbcomp/parser.c"
    اكسر;

  حالة 105: /* RowBody: RowBody RowBodyItem  */
#کرښه 702 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = عدم;}
#کرښه 2954 "src/xkbcomp/parser.c"
    اكسر;

  حالة 106: /* RowBody: RowBodyItem  */
#کرښه 703 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = عدم; }
#کرښه 2960 "src/xkbcomp/parser.c"
    اكسر;

  حالة 107: /* RowBodyItem: "keys" "{" Keys "}" ";"  */
#کرښه 706 "src/xkbcomp/parser.y"
                                                     { (yyval.geom) = عدم; }
#کرښه 2966 "src/xkbcomp/parser.c"
    اكسر;

  حالة 108: /* RowBodyItem: VarDecl  */
#کرښه 708 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = عدم; }
#کرښه 2972 "src/xkbcomp/parser.c"
    اكسر;

  حالة 109: /* Keys: Keys "," Key  */
#کرښه 711 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = عدم; }
#کرښه 2978 "src/xkbcomp/parser.c"
    اكسر;

  حالة 110: /* Keys: Key  */
#کرښه 712 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = عدم; }
#کرښه 2984 "src/xkbcomp/parser.c"
    اكسر;

  حالة 111: /* Key: "key name"  */
#کرښه 716 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 2990 "src/xkbcomp/parser.c"
    اكسر;

  حالة 112: /* Key: "{" ExprList "}"  */
#کرښه 718 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-1].بیان_List).سر); (yyval.geom) = عدم; }
#کرښه 2996 "src/xkbcomp/parser.c"
    اكسر;

  حالة 113: /* OverlayDecl: "overlay" String "{" OverlayKeyList "}" ";"  */
#کرښه 722 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 3002 "src/xkbcomp/parser.c"
    اكسر;

  حالة 114: /* OverlayKeyList: OverlayKeyList "," OverlayKey  */
#کرښه 725 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = عدم; }
#کرښه 3008 "src/xkbcomp/parser.c"
    اكسر;

  حالة 115: /* OverlayKeyList: OverlayKey  */
#کرښه 726 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = عدم; }
#کرښه 3014 "src/xkbcomp/parser.c"
    اكسر;

  حالة 116: /* OverlayKey: "key name" "=" "key name"  */
#کرښه 729 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = عدم; }
#کرښه 3020 "src/xkbcomp/parser.c"
    اكسر;

  حالة 117: /* OutlineList: OutlineList "," OutlineInList  */
#کرښه 733 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم;}
#کرښه 3026 "src/xkbcomp/parser.c"
    اكسر;

  حالة 118: /* OutlineList: OutlineInList  */
#کرښه 735 "src/xkbcomp/parser.y"
                        { (yyval.geom) = عدم; }
#کرښه 3032 "src/xkbcomp/parser.c"
    اكسر;

  حالة 119: /* OutlineInList: "{" CoordList "}"  */
#کرښه 739 "src/xkbcomp/parser.y"
                        { (فراغ) (yyvsp[-1].بیان); (yyval.geom) = عدم; }
#کرښه 3038 "src/xkbcomp/parser.c"
    اكسر;

  حالة 120: /* OutlineInList: Ident "=" "{" CoordList "}"  */
#کرښه 741 "src/xkbcomp/parser.y"
                        { (فراغ) (yyvsp[-1].بیان); (yyval.geom) = عدم; }
#کرښه 3044 "src/xkbcomp/parser.c"
    اكسر;

  حالة 121: /* OutlineInList: Ident "=" Expr  */
#کرښه 743 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].بیان)); (yyval.geom) = عدم; }
#کرښه 3050 "src/xkbcomp/parser.c"
    اكسر;

  حالة 122: /* CoordList: CoordList "," Coord  */
#کرښه 747 "src/xkbcomp/parser.y"
                        { (فراغ) (yyvsp[-2].بیان); (فراغ) (yyvsp[0].بیان); (yyval.بیان) = عدم; }
#کرښه 3056 "src/xkbcomp/parser.c"
    اكسر;

  حالة 123: /* CoordList: Coord  */
#کرښه 749 "src/xkbcomp/parser.y"
                        { (فراغ) (yyvsp[0].بیان); (yyval.بیان) = عدم; }
#کرښه 3062 "src/xkbcomp/parser.c"
    اكسر;

  حالة 124: /* Coord: "[" SignedNumber "," SignedNumber "]"  */
#کرښه 753 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = عدم; }
#کرښه 3068 "src/xkbcomp/parser.c"
    اكسر;

  حالة 125: /* DoodadDecl: DoodadType String "{" VarDeclList "}" ";"  */
#کرښه 757 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-2].varList).سر); (yyval.geom) = عدم; }
#کرښه 3074 "src/xkbcomp/parser.c"
    اكسر;

  حالة 126: /* DoodadType: "text"  */
#کرښه 760 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#کرښه 3080 "src/xkbcomp/parser.c"
    اكسر;

  حالة 127: /* DoodadType: "outline"  */
#کرښه 761 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#کرښه 3086 "src/xkbcomp/parser.c"
    اكسر;

  حالة 128: /* DoodadType: "solid"  */
#کرښه 762 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#کرښه 3092 "src/xkbcomp/parser.c"
    اكسر;

  حالة 129: /* DoodadType: "logo"  */
#کرښه 763 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#کرښه 3098 "src/xkbcomp/parser.c"
    اكسر;

  حالة 130: /* FieldSpec: Ident  */
#کرښه 766 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#کرښه 3104 "src/xkbcomp/parser.c"
    اكسر;

  حالة 131: /* FieldSpec: Element  */
#کرښه 767 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#کرښه 3110 "src/xkbcomp/parser.c"
    اكسر;

  حالة 132: /* Element: "action"  */
#کرښه 771 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "action"); }
#کرښه 3116 "src/xkbcomp/parser.c"
    اكسر;

  حالة 133: /* Element: "interpret"  */
#کرښه 773 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "interpret"); }
#کرښه 3122 "src/xkbcomp/parser.c"
    اكسر;

  حالة 134: /* Element: "type"  */
#کرښه 775 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "type"); }
#کرښه 3128 "src/xkbcomp/parser.c"
    اكسر;

  حالة 135: /* Element: "key"  */
#کرښه 777 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "key"); }
#کرښه 3134 "src/xkbcomp/parser.c"
    اكسر;

  حالة 136: /* Element: "group"  */
#کرښه 779 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "group"); }
#کرښه 3140 "src/xkbcomp/parser.c"
    اكسر;

  حالة 137: /* Element: "modifier_map"  */
#کرښه 781 "src/xkbcomp/parser.y"
                        {(yyval.atom) = xkb_atom_intern_literal(param->ctx, "modifier_map");}
#کرښه 3146 "src/xkbcomp/parser.c"
    اكسر;

  حالة 138: /* Element: "indicator"  */
#کرښه 783 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "indicator"); }
#کرښه 3152 "src/xkbcomp/parser.c"
    اكسر;

  حالة 139: /* Element: "shape"  */
#کرښه 785 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "shape"); }
#کرښه 3158 "src/xkbcomp/parser.c"
    اكسر;

  حالة 140: /* Element: "row"  */
#کرښه 787 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "row"); }
#کرښه 3164 "src/xkbcomp/parser.c"
    اكسر;

  حالة 141: /* Element: "section"  */
#کرښه 789 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "section"); }
#کرښه 3170 "src/xkbcomp/parser.c"
    اكسر;

  حالة 142: /* Element: "text"  */
#کرښه 791 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "text"); }
#کرښه 3176 "src/xkbcomp/parser.c"
    اكسر;

  حالة 143: /* OptMergeMode: MergeMode  */
#کرښه 794 "src/xkbcomp/parser.y"
                                        { (yyval.یوځای) = (yyvsp[0].یوځای); }
#کرښه 3182 "src/xkbcomp/parser.c"
    اكسر;

  حالة 144: /* OptMergeMode: %empty  */
#کرښه 795 "src/xkbcomp/parser.y"
                                        { (yyval.یوځای) = یوځای_DEFAULT; }
#کرښه 3188 "src/xkbcomp/parser.c"
    اكسر;

  حالة 145: /* MergeMode: "include"  */
#کرښه 798 "src/xkbcomp/parser.y"
                                        { (yyval.یوځای) = یوځای_DEFAULT; }
#کرښه 3194 "src/xkbcomp/parser.c"
    اكسر;

  حالة 146: /* MergeMode: "augment"  */
#کرښه 799 "src/xkbcomp/parser.y"
                                        { (yyval.یوځای) = یوځای_AUGMENT; }
#کرښه 3200 "src/xkbcomp/parser.c"
    اكسر;

  حالة 147: /* MergeMode: "override"  */
#کرښه 800 "src/xkbcomp/parser.y"
                                        { (yyval.یوځای) = یوځای_OVERRIDE; }
#کرښه 3206 "src/xkbcomp/parser.c"
    اكسر;

  حالة 148: /* MergeMode: "replace"  */
#کرښه 801 "src/xkbcomp/parser.y"
                                        { (yyval.یوځای) = یوځای_REPLACE; }
#کرښه 3212 "src/xkbcomp/parser.c"
    اكسر;

  حالة 149: /* MergeMode: "alternate"  */
#کرښه 803 "src/xkbcomp/parser.y"
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_پیغام_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    (yyval.یوځای) = یوځای_DEFAULT;
                }
#کرښه 3226 "src/xkbcomp/parser.c"
    اكسر;

  حالة 150: /* ExprList: ExprList "," Expr  */
#کرښه 815 "src/xkbcomp/parser.y"
                        {
                            إذا ((yyvsp[0].بیان)) {
                                إذا ((yyvsp[-2].بیان_List).سر) {
                                    (yyval.بیان_List).سر = (yyvsp[-2].بیان_List).سر;
                                    (yyval.بیان_List).وروستی->common.بل = &(yyvsp[0].بیان)->common;
                                    (yyval.بیان_List).وروستی = (yyvsp[0].بیان);
                                } وإلا {
                                    (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = (yyvsp[0].بیان);
                                }
                            }
                        }
#کرښه 3242 "src/xkbcomp/parser.c"
    اكسر;

  حالة 151: /* ExprList: Expr  */
#کرښه 827 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 3248 "src/xkbcomp/parser.c"
    اكسر;

  حالة 152: /* ExprList: %empty  */
#کرښه 828 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = عدم; }
#کرښه 3254 "src/xkbcomp/parser.c"
    اكسر;

  حالة 153: /* Expr: Expr "/" Expr  */
#کرښه 832 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_دوګونی(STMT_بیان_DIVIDE, (yyvsp[-2].بیان), (yyvsp[0].بیان)); }
#کرښه 3260 "src/xkbcomp/parser.c"
    اكسر;

  حالة 154: /* Expr: Expr "+" Expr  */
#کرښه 834 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_دوګونی(STMT_بیان_جمع_کړه, (yyvsp[-2].بیان), (yyvsp[0].بیان)); }
#کرښه 3266 "src/xkbcomp/parser.c"
    اكسر;

  حالة 155: /* Expr: Expr "-" Expr  */
#کرښه 836 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_دوګونی(STMT_بیان_کمول, (yyvsp[-2].بیان), (yyvsp[0].بیان)); }
#کرښه 3272 "src/xkbcomp/parser.c"
    اكسر;

  حالة 156: /* Expr: Expr "*" Expr  */
#کرښه 838 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_دوګونی(STMT_بیان_MULTIPLY, (yyvsp[-2].بیان), (yyvsp[0].بیان)); }
#کرښه 3278 "src/xkbcomp/parser.c"
    اكسر;

  حالة 157: /* Expr: Lhs "=" Expr  */
#کرښه 840 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_دوګونی(STMT_بیان_ټاکل, (yyvsp[-2].بیان), (yyvsp[0].بیان)); }
#کرښه 3284 "src/xkbcomp/parser.c"
    اكسر;

  حالة 158: /* Expr: Term  */
#کرښه 842 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[0].بیان); }
#کرښه 3290 "src/xkbcomp/parser.c"
    اكسر;

  حالة 159: /* Term: "-" Term  */
#کرښه 846 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Unary(STMT_بیان_NEGATE, (yyvsp[0].بیان)); }
#کرښه 3296 "src/xkbcomp/parser.c"
    اكسر;

  حالة 160: /* Term: "+" Term  */
#کرښه 848 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Unary(STMT_بیان_UNARY_PLUS, (yyvsp[0].بیان)); }
#کرښه 3302 "src/xkbcomp/parser.c"
    اكسر;

  حالة 161: /* Term: "!" Term  */
#کرښه 850 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Unary(STMT_بیان_NOT, (yyvsp[0].بیان)); }
#کرښه 3308 "src/xkbcomp/parser.c"
    اكسر;

  حالة 162: /* Term: "~" Term  */
#کرښه 852 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Unary(STMT_بیان_INVERT, (yyvsp[0].بیان)); }
#کرښه 3314 "src/xkbcomp/parser.c"
    اكسر;

  حالة 163: /* Term: Lhs  */
#کرښه 854 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[0].بیان); }
#کرښه 3320 "src/xkbcomp/parser.c"
    اكسر;

  حالة 164: /* Term: FieldSpec "(" ExprList ")"  */
#کرښه 856 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Action((yyvsp[-3].atom), (yyvsp[-1].بیان_List).سر); }
#کرښه 3326 "src/xkbcomp/parser.c"
    اكسر;

  حالة 165: /* Term: Actions  */
#کرښه 858 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[0].بیان); }
#کرښه 3332 "src/xkbcomp/parser.c"
    اكسر;

  حالة 166: /* Term: Terminal  */
#کرښه 860 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[0].بیان); }
#کرښه 3338 "src/xkbcomp/parser.c"
    اكسر;

  حالة 167: /* Term: "(" Expr ")"  */
#کرښه 862 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[-1].بیان); }
#کرښه 3344 "src/xkbcomp/parser.c"
    اكسر;

  حالة 168: /* MultiActionList: MultiActionList "," Action  */
#کرښه 866 "src/xkbcomp/parser.y"
                        {
                            بیان_Def *بیان = بیان_Create_Action_List((yyvsp[0].بیان));
                            (yyval.بیان_List) = (yyvsp[-2].بیان_List);
                            (yyval.بیان_List).وروستی->common.بل = &بیان->common; (yyval.بیان_List).وروستی = بیان;
                        }
#کرښه 3354 "src/xkbcomp/parser.c"
    اكسر;

  حالة 169: /* MultiActionList: MultiActionList "," Actions  */
#کرښه 872 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List) = (yyvsp[-2].بیان_List); (yyval.بیان_List).وروستی->common.بل = &(yyvsp[0].بیان)->common; (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 3360 "src/xkbcomp/parser.c"
    اكسر;

  حالة 170: /* MultiActionList: Action  */
#کرښه 874 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = بیان_Create_Action_List((yyvsp[0].بیان)); }
#کرښه 3366 "src/xkbcomp/parser.c"
    اكسر;

  حالة 171: /* MultiActionList: NonEmptyActions  */
#کرښه 876 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 3372 "src/xkbcomp/parser.c"
    اكسر;

  حالة 172: /* ActionList: ActionList "," Action  */
#کرښه 880 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List) = (yyvsp[-2].بیان_List); (yyval.بیان_List).وروستی->common.بل = &(yyvsp[0].بیان)->common; (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 3378 "src/xkbcomp/parser.c"
    اكسر;

  حالة 173: /* ActionList: Action  */
#کرښه 882 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 3384 "src/xkbcomp/parser.c"
    اكسر;

  حالة 174: /* NonEmptyActions: "{" ActionList "}"  */
#کرښه 886 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Action_List((yyvsp[-1].بیان_List).سر); }
#کرښه 3390 "src/xkbcomp/parser.c"
    اكسر;

  حالة 175: /* Actions: NonEmptyActions  */
#کرښه 890 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[0].بیان); }
#کرښه 3396 "src/xkbcomp/parser.c"
    اكسر;

  حالة 176: /* Actions: "{" "}"  */
#کرښه 892 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Action_List(عدم); }
#کرښه 3402 "src/xkbcomp/parser.c"
    اكسر;

  حالة 177: /* Action: FieldSpec "(" ExprList ")"  */
#کرښه 896 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Action((yyvsp[-3].atom), (yyvsp[-1].بیان_List).سر); }
#کرښه 3408 "src/xkbcomp/parser.c"
    اكسر;

  حالة 178: /* Lhs: FieldSpec  */
#کرښه 900 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Ident((yyvsp[0].atom)); }
#کرښه 3414 "src/xkbcomp/parser.c"
    اكسر;

  حالة 179: /* Lhs: FieldSpec "." FieldSpec  */
#کرښه 902 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_ډګر_Ref((yyvsp[-2].atom), (yyvsp[0].atom)); }
#کرښه 3420 "src/xkbcomp/parser.c"
    اكسر;

  حالة 180: /* Lhs: FieldSpec "[" Expr "]"  */
#کرښه 904 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Array_Ref(XKB_ATOM_NONE, (yyvsp[-3].atom), (yyvsp[-1].بیان)); }
#کرښه 3426 "src/xkbcomp/parser.c"
    اكسر;

  حالة 181: /* Lhs: FieldSpec "." FieldSpec "[" Expr "]"  */
#کرښه 906 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Array_Ref((yyvsp[-5].atom), (yyvsp[-3].atom), (yyvsp[-1].بیان)); }
#کرښه 3432 "src/xkbcomp/parser.c"
    اكسر;

  حالة 182: /* OptTerminal: Terminal  */
#کرښه 910 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[0].بیان); }
#کرښه 3438 "src/xkbcomp/parser.c"
    اكسر;

  حالة 183: /* OptTerminal: %empty  */
#کرښه 911 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = عدم; }
#کرښه 3444 "src/xkbcomp/parser.c"
    اكسر;

  حالة 184: /* Terminal: String  */
#کرښه 915 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_String((yyvsp[0].atom)); }
#کرښه 3450 "src/xkbcomp/parser.c"
    اكسر;

  حالة 185: /* Terminal: Integer  */
#کرښه 917 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Integer((yyvsp[0].عدد)); }
#کرښه 3456 "src/xkbcomp/parser.c"
    اكسر;

  حالة 186: /* Terminal: Float  */
#کرښه 919 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_Float(/* Discard $1 */); }
#کرښه 3462 "src/xkbcomp/parser.c"
    اكسر;

  حالة 187: /* Terminal: "key name"  */
#کرښه 921 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_کلی_Name((yyvsp[0].atom)); }
#کرښه 3468 "src/xkbcomp/parser.c"
    اكسر;

  حالة 188: /* MultiKeySymList: MultiKeySymList "," KeySymLit  */
#کرښه 925 "src/xkbcomp/parser.y"
                        {
                            بیان_Def *بیان = بیان_Create_کلی_Sym_List((yyvsp[0].keysym));
                            (yyval.بیان_List) = (yyvsp[-2].بیان_List);
                            (yyval.بیان_List).وروستی->common.بل = &بیان->common; (yyval.بیان_List).وروستی = بیان;
                        }
#کرښه 3478 "src/xkbcomp/parser.c"
    اكسر;

  حالة 189: /* MultiKeySymList: MultiKeySymList "," KeySyms  */
#کرښه 931 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List) = (yyvsp[-2].بیان_List); (yyval.بیان_List).وروستی->common.بل = &(yyvsp[0].بیان)->common; (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 3484 "src/xkbcomp/parser.c"
    اكسر;

  حالة 190: /* MultiKeySymList: KeySymLit  */
#کرښه 933 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = بیان_Create_کلی_Sym_List((yyvsp[0].keysym)); }
#کرښه 3490 "src/xkbcomp/parser.c"
    اكسر;

  حالة 191: /* MultiKeySymList: NonEmptyKeySyms  */
#کرښه 935 "src/xkbcomp/parser.y"
                        { (yyval.بیان_List).سر = (yyval.بیان_List).وروستی = (yyvsp[0].بیان); }
#کرښه 3496 "src/xkbcomp/parser.c"
    اكسر;

  حالة 192: /* KeySymList: KeySymList "," KeySymLit  */
#کرښه 939 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Append_کلی_Sym_List((yyvsp[-2].بیان), (yyvsp[0].keysym)); }
#کرښه 3502 "src/xkbcomp/parser.c"
    اكسر;

  حالة 193: /* KeySymList: KeySymList "," "string literal"  */
#کرښه 941 "src/xkbcomp/parser.y"
                        {
                            (yyval.بیان) = بیان_کلی_Sym_List_Append_String(param->scanner, (yyvsp[-2].بیان), (yyvsp[0].str));
                            حرر((yyvsp[0].str));
                            إذا (!(yyval.بیان))
                                YYERROR;
                        }
#کرښه 3513 "src/xkbcomp/parser.c"
    اكسر;

  حالة 194: /* KeySymList: KeySymLit  */
#کرښه 948 "src/xkbcomp/parser.y"
                        {
                            (yyval.بیان) = بیان_Create_کلی_Sym_List((yyvsp[0].keysym));
                            إذا (!(yyval.بیان))
                                YYERROR;
                        }
#کرښه 3523 "src/xkbcomp/parser.c"
    اكسر;

  حالة 195: /* KeySymList: "string literal"  */
#کرښه 954 "src/xkbcomp/parser.y"
                        {
                            (yyval.بیان) = بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه);
                            إذا (!(yyval.بیان))
                                YYERROR;
                            (yyval.بیان) = بیان_کلی_Sym_List_Append_String(param->scanner, (yyval.بیان), (yyvsp[0].str));
                            حرر((yyvsp[0].str));
                            إذا (!(yyval.بیان))
                                YYERROR;
                        }
#کرښه 3537 "src/xkbcomp/parser.c"
    اكسر;

  حالة 196: /* NonEmptyKeySyms: "{" KeySymList "}"  */
#کرښه 966 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[-1].بیان); }
#کرښه 3543 "src/xkbcomp/parser.c"
    اكسر;

  حالة 197: /* NonEmptyKeySyms: "string literal"  */
#کرښه 968 "src/xkbcomp/parser.y"
                        {
                            (yyval.بیان) = بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه);
                            إذا (!(yyval.بیان))
                                YYERROR;
                            (yyval.بیان) = بیان_کلی_Sym_List_Append_String(param->scanner, (yyval.بیان), (yyvsp[0].str));
                            حرر((yyvsp[0].str));
                            إذا (!(yyval.بیان))
                                YYERROR;
                        }
#کرښه 3557 "src/xkbcomp/parser.c"
    اكسر;

  حالة 198: /* KeySyms: NonEmptyKeySyms  */
#کرښه 980 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = (yyvsp[0].بیان); }
#کرښه 3563 "src/xkbcomp/parser.c"
    اكسر;

  حالة 199: /* KeySyms: "{" "}"  */
#کرښه 982 "src/xkbcomp/parser.y"
                        { (yyval.بیان) = بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه); }
#کرښه 3569 "src/xkbcomp/parser.c"
    اكسر;

  حالة 200: /* KeySym: KeySymLit  */
#کرښه 986 "src/xkbcomp/parser.y"
                        { (yyval.keysym) = (yyvsp[0].keysym); }
#کرښه 3575 "src/xkbcomp/parser.c"
    اكسر;

  حالة 201: /* KeySym: "string literal"  */
#کرښه 988 "src/xkbcomp/parser.y"
                        {
                            (yyval.keysym) = KeysymParseString(param->scanner, (yyvsp[0].str));
                            حرر((yyvsp[0].str));
                            إذا ((yyval.keysym) == XKB_کلی_No_نښه)
                                YYERROR;
                        }
#کرښه 3586 "src/xkbcomp/parser.c"
    اكسر;

  حالة 202: /* KeySymLit: "identifier"  */
#کرښه 997 "src/xkbcomp/parser.y"
                        {
                            إذا (!resolve_keysym(param, (yyvsp[0].sval), &(yyval.keysym))) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (بینښه صحيح) (yyvsp[0].sval).len, (yyvsp[0].sval).پیل
                                );
                                (yyval.keysym) = XKB_کلی_No_نښه;
                            }
                        }
#کرښه 3602 "src/xkbcomp/parser.c"
    اكسر;

  حالة 203: /* KeySymLit: "section"  */
#کرښه 1009 "src/xkbcomp/parser.y"
                                { (yyval.keysym) = XKB_کلی_section; }
#کرښه 3608 "src/xkbcomp/parser.c"
    اكسر;

  حالة 204: /* KeySymLit: "decimal digit"  */
#کرښه 1011 "src/xkbcomp/parser.y"
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            (yyval.keysym) = XKB_کلی_0 + (xkb_keysym_t) (yyvsp[0].عدد);
                        }
#کرښه 3621 "src/xkbcomp/parser.c"
    اكسر;

  حالة 205: /* KeySymLit: "integer literal"  */
#کرښه 1020 "src/xkbcomp/parser.y"
                        {
                            إذا ((yyvsp[0].عدد) < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -(yyvsp[0].عدد), (yyvsp[0].عدد)
                                );
                                (yyval.keysym) = XKB_کلی_No_نښه;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            وإلا {
                                /* Any other numeric value */
                                إذا ((yyvsp[0].عدد) <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    (yyval.keysym) = (xkb_keysym_t) (yyvsp[0].عدد);
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        (yyval.keysym), عدم, (yyval.keysym), "%#06"PRIx32, "");
                                } وإلا {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", (yyvsp[0].عدد), (yyvsp[0].عدد)
                                    );
                                    (yyval.keysym) = XKB_کلی_No_نښه;
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
                                    (yyvsp[0].عدد), (yyvsp[0].عدد)
                                );
                            }
                        }
#کرښه 3683 "src/xkbcomp/parser.c"
    اكسر;

  حالة 206: /* SignedNumber: "-" Number  */
#کرښه 1079 "src/xkbcomp/parser.y"
                                        { (yyval.عدد) = -(yyvsp[0].عدد); }
#کرښه 3689 "src/xkbcomp/parser.c"
    اكسر;

  حالة 207: /* SignedNumber: Number  */
#کرښه 1080 "src/xkbcomp/parser.y"
                                        { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3695 "src/xkbcomp/parser.c"
    اكسر;

  حالة 208: /* Number: "float literal"  */
#کرښه 1083 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3701 "src/xkbcomp/parser.c"
    اكسر;

  حالة 209: /* Number: "decimal digit"  */
#کرښه 1084 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3707 "src/xkbcomp/parser.c"
    اكسر;

  حالة 210: /* Number: "integer literal"  */
#کرښه 1085 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3713 "src/xkbcomp/parser.c"
    اكسر;

  حالة 211: /* Float: "float literal"  */
#کرښه 1088 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#کرښه 3719 "src/xkbcomp/parser.c"
    اكسر;

  حالة 212: /* Integer: "integer literal"  */
#کرښه 1091 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3725 "src/xkbcomp/parser.c"
    اكسر;

  حالة 213: /* Integer: "decimal digit"  */
#کرښه 1092 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3731 "src/xkbcomp/parser.c"
    اكسر;

  حالة 214: /* KeyCode: "integer literal"  */
#کرښه 1095 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3737 "src/xkbcomp/parser.c"
    اكسر;

  حالة 215: /* KeyCode: "decimal digit"  */
#کرښه 1096 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#کرښه 3743 "src/xkbcomp/parser.c"
    اكسر;

  حالة 216: /* Ident: "identifier"  */
#کرښه 1099 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].sval).پیل, (yyvsp[0].sval).len); }
#کرښه 3749 "src/xkbcomp/parser.c"
    اكسر;

  حالة 217: /* Ident: "default"  */
#کرښه 1100 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "default"); }
#کرښه 3755 "src/xkbcomp/parser.c"
    اكسر;

  حالة 218: /* String: "string literal"  */
#کرښه 1103 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].str), مزي_اوږدوالی((yyvsp[0].str))); حرر((yyvsp[0].str)); }
#کرښه 3761 "src/xkbcomp/parser.c"
    اكسر;

  حالة 219: /* OptMapName: MapName  */
#کرښه 1106 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#کرښه 3767 "src/xkbcomp/parser.c"
    اكسر;

  حالة 220: /* OptMapName: %empty  */
#کرښه 1107 "src/xkbcomp/parser.y"
                                { (yyval.str) = عدم; }
#کرښه 3773 "src/xkbcomp/parser.c"
    اكسر;

  حالة 221: /* MapName: "string literal"  */
#کرښه 1110 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#کرښه 3779 "src/xkbcomp/parser.c"
    اكسر;


#کرښه 3783 "src/xkbcomp/parser.c"

      افتراضي: اكسر;
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
  YY_نښه_PRINT ("-> $$ =", YY_CAST (yysymbol_ډول_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ثابت صحيح yylhs = yyr1[yyn] - YYNTOKENS;
    ثابت صحيح yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  اذهب yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  إذا (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        حرف ثابت *yymsgp = YY_("syntax error");
        صحيح yysyntax_error_حالت;
        yysyntax_error_حالت = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        إذا (yysyntax_error_حالت == 0)
          yymsgp = yymsg;
        وإلا إذا (yysyntax_error_حالت == -1)
          {
            إذا (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (حرف *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            إذا (yymsg)
              {
                yysyntax_error_حالت
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            وإلا
              {
                yymsg = yymsgbuf;
                yymsg_alloc = حجم yymsgbuf;
                yysyntax_error_حالت = YYENOMEM;
              }
          }
        yyerror (param, yymsgp);
        إذا (yysyntax_error_حالت == YYENOMEM)
          YYNOMEM;
      }
    }

  إذا (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      إذا (yychar <= پای_ټکی_د_FILE)
        {
          /* Return failure if at end of input.  */
          إذا (yychar == پای_ټکی_د_FILE)
            YYABORT;
        }
      وإلا
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, param);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  اذهب yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  إذا (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ډېری_PRINT (yyss, yyssp);
  yystate = *yyssp;
  اذهب yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  لكل (;;)
    {
      yyn = yypact[yystate];
      إذا (!yypact_ارزښت_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          إذا (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              إذا (0 < yyn)
                اكسر;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      إذا (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_نښه (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_ډېری_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_پای_ټکی


  /* Shift the error token.  */
  YY_نښه_PRINT ("Shifting", YY_ACCESSING_نښه (yyn), yyvsp, yylsp);

  yystate = yyn;
  اذهب yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  اذهب yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  اذهب yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  اذهب yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  إذا (yychar != YYEMPTY)
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
  YY_ډېری_PRINT (yyss, yyssp);
  طالما (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_نښه (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#که_نه_تعریف_شوی yyoverflow
  إذا (yyss != yyssa)
    YYSTACK_FREE (yyss);
#د_که_پای
  إذا (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  أرجع yyresult;
}

#کرښه 1113 "src/xkbcomp/parser.y"


/* Parse a specific section */
XkbFile *
parse(بنية xkb_context *ctx, ثابت بنية parser_keymap_config *config,
      بنية scanner *scanner, ثابت حرف *map)
{
    صحيح ret;
    XkbFile *لومړی = عدم;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .نور_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    طالما ((ret = yyparse(&param)) == 0 && param.نور_maps) {
        إذا (map) {
            إذا (streq_not_null(map, param.rtrn->name))
                أرجع param.rtrn;
            وإلا
                FreeXkbFile(param.rtrn);
        }
        وإلا {
            إذا (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(لومړی);
                أرجع param.rtrn;
            }
            وإلا إذا (!لومړی) {
                لومړی = param.rtrn;
            }
            وإلا {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = عدم;
    }

    إذا (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(لومړی);
        FreeXkbFile(param.rtrn);
        أرجع عدم;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    إذا (لومړی && لومړی->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, خوندي_map_name(لومړی->name));

    أرجع لومړی;
}

/* Parse the next section */
bool
parse_بل(بنية xkb_context *ctx,
           ثابت بنية parser_keymap_config *config,
           بنية scanner *scanner, XkbFile **xkb_file)
{
    صحيح ret;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .نور_maps = false,
    };

    إذا ((ret = yyparse(&param)) == 0 && param.نور_maps) {
        *xkb_file = param.rtrn;
        أرجع true;
    } وإلا {
        FreeXkbFile(param.rtrn);
        *xkb_file = عدم;
        أرجع (ret == 0);
    }
}
