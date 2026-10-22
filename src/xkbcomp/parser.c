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
#تعریف_کرو YYBISON 30802

/* Bison version string.  */
#تعریف_کرو YYBISON_نسخہ "3.8.2"

/* Skeleton name.  */
#تعریف_کرو YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#تعریف_کرو YYPURE 1

/* Push parsers.  */
#تعریف_کرو YYPUSH 0

/* Pull parsers.  */
#تعریف_کرو YYPULL 1


/* Substitute the variable and function names.  */
#تعریف_کرو yyparse         _xkbcommon_parse
#تعریف_کرو yylex           _xkbcommon_lex
#تعریف_کرو yyerror         _xkbcommon_error
#تعریف_کرو yydebug         _xkbcommon_debug
#تعریف_کرو yynerrs         _xkbcommon_nerrs

/* First part of user prologue.  */
#سطر 22 "src/xkbcomp/parser.y"

#شامل "config.h"

#شامل <assert.h>

#شامل "keysym.h"
#شامل "xkbcomp/xkbcomp-priv.h"
#شامل "xkbcomp/parser-priv.h"
#شامل "xkbcomp/ast-build.h"

ساخت parser_param {
    ساخت xkb_context *ctx;
    ساخت scanner *scanner;
    XkbFile *rtrn;
    ساخت parser_keymap_config config;
    bool مزید_maps;
};

#تعریف_کرو parser_log_with_رمز(param, level, verbosity, log_پیغام_id, fmt, ...)   \
    scanner_log_with_رمز((param)->scanner, level, verbosity, log_پیغام_id, fmt,\
                          ##__VA_ARGS__)

#تعریف_کرو parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#تعریف_کرو parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#تعریف_کرو parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ساکن خالی
_xkbcommon_error(ساخت parser_param *param, ثابت حرف *پیغام)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", پیغام);
}

ساکن bool
resolve_keysym(ساخت parser_param *param, ساخت sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    اگر (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_کلید_No_علامت;
        واپس true;
    }

    اگر (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_کلید_Void_علامت;
        واپس true;
    }

    /* xkb_keysym_from_name needs a C string. */
    حرف buf[XKB_KEYSYM_NAME_MAX_سائز];
    اگر (name.len >= حجم(buf)) {
        واپس false;
    }
    حافظہ_نقل(buf, name.آغاز, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    اگر (sym != XKB_کلید_No_علامت) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        واپس true;
    }

    واپس false;
}

#تعریف_کرو param_scanner param->scanner

#سطر 150 "src/xkbcomp/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  ورنہ
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  اگر defined __cplusplus
#   اگر 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   ورنہ
#    define YY_NULLPTR 0
#   endif
#  ورنہ
#   define YY_NULLPTR ((خالی*)0)
#  endif
# endif

#شامل "parser.h"
/* Symbol kind.  */
شمار yysymbol_قسم_t
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
  YYSYMBOL_کلید = 21,                       /* "key"  */
  YYSYMBOL_ALIAS = 22,                     /* "alias"  */
  YYSYMBOL_گروہ = 23,                     /* "group"  */
  YYSYMBOL_MODIFIER_MAP = 24,              /* "modifier_map"  */
  YYSYMBOL_INDICATOR = 25,                 /* "indicator"  */
  YYSYMBOL_SHAPE = 26,                     /* "shape"  */
  YYSYMBOL_کلیدیں = 27,                      /* "keys"  */
  YYSYMBOL_صف = 28,                       /* "row"  */
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
  YYSYMBOL_ALPHANUMERIC_کلیدیں = 61,         /* "alphanumeric_keys"  */
  YYSYMBOL_MODIFIER_کلیدیں = 62,             /* "modifier_keys"  */
  YYSYMBOL_KEYPAD_کلیدیں = 63,               /* "keypad_keys"  */
  YYSYMBOL_FUNCTION_کلیدیں = 64,             /* "function_keys"  */
  YYSYMBOL_ALTERNATE_گروہ = 65,           /* "alternate_group"  */
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
  YYSYMBOL_کلید_Name_Decl = 80,               /* KeyNameDecl  */
  YYSYMBOL_کلید_Alias_Decl = 81,              /* KeyAliasDecl  */
  YYSYMBOL_VModDecl = 82,                  /* VModDecl  */
  YYSYMBOL_VModDefList = 83,               /* VModDefList  */
  YYSYMBOL_VModDef = 84,                   /* VModDef  */
  YYSYMBOL_InterpretDecl = 85,             /* InterpretDecl  */
  YYSYMBOL_Interpret_مطابقت = 86,            /* InterpretMatch  */
  YYSYMBOL_VarDeclList = 87,               /* VarDeclList  */
  YYSYMBOL_کلید_Type_Decl = 88,               /* KeyTypeDecl  */
  YYSYMBOL_SymbolsDecl = 89,               /* SymbolsDecl  */
  YYSYMBOL_OptSymbolsBody = 90,            /* OptSymbolsBody  */
  YYSYMBOL_SymbolsBody = 91,               /* SymbolsBody  */
  YYSYMBOL_SymbolsVarDecl = 92,            /* SymbolsVarDecl  */
  YYSYMBOL_Multi_کلید_Sym_Or_Action_List = 93,   /* MultiKeySymOrActionList  */
  YYSYMBOL_No_علامت_Or_Action_List = 94,      /* NoSymbolOrActionList  */
  YYSYMBOL_گروہ_Compat_Decl = 95,           /* GroupCompatDecl  */
  YYSYMBOL_ModMapDecl = 96,                /* ModMapDecl  */
  YYSYMBOL_کلید_Or_کلید_Sym_List = 97,           /* KeyOrKeySymList  */
  YYSYMBOL_کلید_Or_کلید_Sym = 98,               /* KeyOrKeySym  */
  YYSYMBOL_LedMapDecl = 99,                /* LedMapDecl  */
  YYSYMBOL_LedNameDecl = 100,              /* LedNameDecl  */
  YYSYMBOL_UnknownDecl = 101,              /* UnknownDecl  */
  YYSYMBOL_UnknownCompoundStatementDecl = 102, /* UnknownCompoundStatementDecl  */
  YYSYMBOL_ShapeDecl = 103,                /* ShapeDecl  */
  YYSYMBOL_SectionDecl = 104,              /* SectionDecl  */
  YYSYMBOL_SectionBody = 105,              /* SectionBody  */
  YYSYMBOL_Section_Body_شے = 106,          /* SectionBodyItem  */
  YYSYMBOL_صف_Body = 107,                  /* RowBody  */
  YYSYMBOL_صف_Body_شے = 108,              /* RowBodyItem  */
  YYSYMBOL_کلیدیں_2 = 109,                     /*کلیدیں*/
  YYSYMBOL_کلید_2 = 110,                      /*کلید*/
  YYSYMBOL_OverlayDecl = 111,              /* OverlayDecl  */
  YYSYMBOL_Overlay_کلید_List = 112,           /* OverlayKeyList  */
  YYSYMBOL_Overlay_کلید = 113,               /* OverlayKey  */
  YYSYMBOL_OutlineList = 114,              /* OutlineList  */
  YYSYMBOL_Outline_اندر_List = 115,            /* OutlineInList  */
  YYSYMBOL_CoordList = 116,                /* CoordList  */
  YYSYMBOL_Coord = 117,                    /* Coord  */
  YYSYMBOL_DoodadDecl = 118,               /* DoodadDecl  */
  YYSYMBOL_DoodadType = 119,               /* DoodadType  */
  YYSYMBOL_میدان_Spec = 120,                /* FieldSpec  */
  YYSYMBOL_Element = 121,                  /* Element  */
  YYSYMBOL_Opt_ملاؤ_منوال = 122,             /* OptMergeMode  */
  YYSYMBOL_ملاؤ_منوال = 123,                /* MergeMode  */
  YYSYMBOL_اظہاریہ_List = 124,                 /* ExprList  */
  YYSYMBOL_اظہاریہ = 125,                     /*اظہاریہ*/
  YYSYMBOL_رکن = 126,                     /*رکن*/
  YYSYMBOL_MultiActionList = 127,          /* MultiActionList  */
  YYSYMBOL_ActionList = 128,               /* ActionList  */
  YYSYMBOL_Non_خالی_خانے_Actions = 129,          /* NonEmptyActions  */
  YYSYMBOL_Actions = 130,                  /* Actions  */
  YYSYMBOL_Action = 131,                   /* Action  */
  YYSYMBOL_Lhs = 132,                      /* Lhs  */
  YYSYMBOL_OptTerminal = 133,              /* OptTerminal  */
  YYSYMBOL_Terminal = 134,                 /* Terminal  */
  YYSYMBOL_Multi_کلید_Sym_List = 135,          /* MultiKeySymList  */
  YYSYMBOL_کلید_Sym_List = 136,               /* KeySymList  */
  YYSYMBOL_Non_خالی_خانے_کلید_Syms = 137,          /* NonEmptyKeySyms  */
  YYSYMBOL_کلید_Syms = 138,                  /* KeySyms  */
  YYSYMBOL_کلید_Sym = 139,                   /* KeySym  */
  YYSYMBOL_کلید_Sym_Lit = 140,                /* KeySymLit  */
  YYSYMBOL_Signed_نمبر = 141,             /* SignedNumber  */
  YYSYMBOL_نمبر = 142,                   /*نمبر*/
  YYSYMBOL_Float = 143,                    /* Float  */
  YYSYMBOL_Integer = 144,                  /* Integer  */
  YYSYMBOL_کلید_رمز = 145,                  /* KeyCode  */
  YYSYMBOL_Ident = 146,                    /* Ident  */
  YYSYMBOL_String = 147,                   /* String  */
  YYSYMBOL_OptMapName = 148,               /* OptMapName  */
  YYSYMBOL_MapName = 149                   /* MapName  */
};
قسمتعریف شمار yysymbol_قسم_t yysymbol_قسم_t;




#اگر_تعریف_شدہ مختصر
# undef مختصر
#اگر_ختم

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#اگر_غیر_تعریف_شدہ __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# اگر defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#اگر_ختم

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#اگر_تعریف_شدہ __INT_LEAST8_MAX__
قسمتعریف __INT_LEAST8_TYPE__ yytype_int8;
#وگرنہ_اگر defined YY_STDINT_H
قسمتعریف int_least8_t yytype_int8;
#ورنہ
قسمتعریف علامتی حرف yytype_int8;
#اگر_ختم

#اگر_تعریف_شدہ __INT_LEAST16_MAX__
قسمتعریف __INT_LEAST16_TYPE__ yytype_int16;
#وگرنہ_اگر defined YY_STDINT_H
قسمتعریف int_least16_t yytype_int16;
#ورنہ
قسمتعریف مختصر yytype_int16;
#اگر_ختم

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#اگر_تعریف_شدہ __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#اگر_ختم

#اگر defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
قسمتعریف __UINT_LEAST8_TYPE__ yytype_uint8;
#وگرنہ_اگر (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= زیادہ_سے_زیادہ_عدد)
قسمتعریف uint_least8_t yytype_uint8;
#وگرنہ_اگر !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= زیادہ_سے_زیادہ_عدد
قسمتعریف غیرعلامتی حرف yytype_uint8;
#ورنہ
قسمتعریف مختصر yytype_uint8;
#اگر_ختم

#اگر defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
قسمتعریف __UINT_LEAST16_TYPE__ yytype_uint16;
#وگرنہ_اگر (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= زیادہ_سے_زیادہ_عدد)
قسمتعریف uint_least16_t yytype_uint16;
#وگرنہ_اگر !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= زیادہ_سے_زیادہ_عدد
قسمتعریف غیرعلامتی مختصر yytype_uint16;
#ورنہ
قسمتعریف صحیح yytype_uint16;
#اگر_ختم

#اگر_غیر_تعریف_شدہ YYPTRDIFF_T
# اگر defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_زیادہ_سے_زیادہ __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_زیادہ_سے_زیادہ PTRDIFF_MAX
# ورنہ
#  define YYPTRDIFF_T طویل
#  define YYPTRDIFF_زیادہ_سے_زیادہ LONG_MAX
# endif
#اگر_ختم

#اگر_غیر_تعریف_شدہ YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined سائز_t
#  define YYSIZE_T سائز_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T سائز_t
# ورنہ
#  define YYSIZE_T غیرعلامتی
# endif
#اگر_ختم

#تعریف_کرو YYSIZE_زیادہ_سے_زیادہ                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_زیادہ_سے_زیادہ < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_زیادہ_سے_زیادہ                         \
            : YY_CAST (YYSIZE_T, -1)))

#تعریف_کرو YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, حجم (X))


/* Stored state numbers (used for stacks). */
قسمتعریف yytype_int16 yy_state_t;

/* State numbers in computations.  */
قسمتعریف صحیح yy_state_fast_t;

#اگر_غیر_تعریف_شدہ YY_
# اگر defined YYENABLE_NLS && YYENABLE_NLS
#  اگر ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#اگر_ختم


#اگر_غیر_تعریف_شدہ YY_ATTRIBUTE_خالص
# اگر defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_خالص __attribute__ ((__pure__))
# ورنہ
#  define YY_ATTRIBUTE_خالص
# endif
#اگر_ختم

#اگر_غیر_تعریف_شدہ YY_ATTRIBUTE_غیر_مستعمل
# اگر defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_غیر_مستعمل __attribute__ ((__unused__))
# ورنہ
#  define YY_ATTRIBUTE_غیر_مستعمل
# endif
#اگر_ختم

/* Suppress unused-variable warnings by "using" E.  */
#اگر ! defined lint || defined __GNUC__
# define YY_USE(E) ((خالی) (E))
#ورنہ
# define YY_USE(E) /*خالی_خانے*/
#اگر_ختم

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#اگر defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# اگر __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# ورنہ
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_انتہا      \
    _Pragma ("GCC diagnostic pop")
#ورنہ
# define YY_INITIAL_قدر(قدر_2) قدر_2
#اگر_ختم
#اگر_غیر_تعریف_شدہ YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_انتہا
#اگر_ختم
#اگر_غیر_تعریف_شدہ YY_INITIAL_قدر
# define YY_INITIAL_قدر(قدر_2) /* Nothing. */
#اگر_ختم

#اگر defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_انتہا            \
    _Pragma ("GCC diagnostic pop")
#اگر_ختم
#اگر_غیر_تعریف_شدہ YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_انتہا
#اگر_ختم


#تعریف_کرو YY_ASSERT(E) ((خالی) (0 && (E)))

#اگر 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  اگر YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   ورنہ
#    define YYSTACK_ALLOC alloca
#    اگر ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
#  define YYSTACK_FREE(Ptr) کرو { /*خالی_خانے*/; } جب (0)
#  ifndef YYSTACK_ALLOC_زیادہ_سے_زیادہ
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_زیادہ_سے_زیادہ 4032 /* reasonable circa 2006 */
#  endif
# ورنہ
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_زیادہ_سے_زیادہ
#   define YYSTACK_ALLOC_زیادہ_سے_زیادہ YYSIZE_زیادہ_سے_زیادہ
#  endif
#  اگر (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined حافظہ_دو) \
             && (defined YYFREE || defined آزاد)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC حافظہ_دو
#   اگر ! defined حافظہ_دو && ! defined EXIT_SUCCESS
خالی *حافظہ_دو (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE آزاد
#   اگر ! defined آزاد && ! defined EXIT_SUCCESS
خالی آزاد (خالی *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#اگر_ختم /* 1 */

#اگر (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
اتحاد yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_زیادہ_سے_زیادہ (YYSIZEOF (اتحاد yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_بائٹیں(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_زیادہ_سے_زیادہ)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(ڈھیر_alloc, ڈھیر_2)                           \
    کرو                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ڈھیر_alloc, ڈھیر_2, yysize);                    \
        ڈھیر_2 = &yyptr->ڈھیر_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ڈھیر_2) + YYSTACK_GAP_زیادہ_سے_زیادہ; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    جب (0)

#اگر_ختم

#اگر defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  اگر defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, گنتی_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (گنتی_2)) * حجم (*(Src)))
#  ورنہ
#   define YYCOPY(Dst, Src, گنتی_2)              \
      کرو                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          برائے (yyi = 0; yyi < (گنتی_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      جب (0)
#  endif
# endif
#اگر_ختم /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#تعریف_کرو YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#تعریف_کرو YYLAST   945

/* YYNTOKENS -- Number of terminals.  */
#تعریف_کرو YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#تعریف_کرو YYNNTS  83
/* YYNRULES -- Number of rules.  */
#تعریف_کرو YYNRULES  221
/* YYNSTATES -- Number of states.  */
#تعریف_کرو YYNSTATES  386

/* YYMAXUTOK -- Last valid token kind.  */
#تعریف_کرو YYMAXUTOK   257


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#تعریف_کرو YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_قسم_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
ساکن ثابت yytype_int8 yytranslate[] =
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

#اگر YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
ساکن ثابت yytype_int16 yyrline[] =
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
#اگر_ختم

/** Accessing symbol of state STATE.  */
#تعریف_کرو YY_ACCESSING_علامت(State) YY_CAST (yysymbol_قسم_t, yystos[State])

#اگر 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
ساکن ثابت حرف *yysymbol_name (yysymbol_قسم_t yysymbol) YY_ATTRIBUTE_غیر_مستعمل;

ساکن ثابت حرف *
yysymbol_name (yysymbol_قسم_t yysymbol)
{
  ساکن ثابت حرف *ثابت yy_sname[] =
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
  واپس yy_sname[yysymbol];
}
#اگر_ختم

#تعریف_کرو YYPACT_NINF (-225)

#تعریف_کرو yypact_قدر_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#تعریف_کرو YYTABLE_NINF (-217)

#تعریف_کرو yytable_قدر_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
ساکن ثابت yytype_int16 yypact[] =
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
ساکن ثابت yytype_uint8 yydefact[] =
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
ساکن ثابت yytype_int16 yypgoto[] =
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
ساکن ثابت yytype_int16 yydefgoto[] =
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
ساکن ثابت yytype_int16 yytable[] =
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

ساکن ثابت yytype_int16 yycheck[] =
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
ساکن ثابت yytype_uint8 yystos[] =
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
ساکن ثابت yytype_uint8 yyr1[] =
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
ساکن ثابت yytype_int8 yyr2[] =
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


شمار { YYENOMEM = -2 };

#تعریف_کرو yyerrok         (yyerrstatus = 0)
#تعریف_کرو yyclearin       (yychar = YYEMPTY)

#تعریف_کرو YYACCEPT        جاؤ yyacceptlab
#تعریف_کرو YYABORT         جاؤ yyabortlab
#تعریف_کرو YYERROR         جاؤ yyerrorlab
#تعریف_کرو YYNOMEM         جاؤ yyexhaustedlab


#تعریف_کرو YYRECOVERING()  (!!yyerrstatus)

#تعریف_کرو YYBACKUP(Token, قدر_2)                                    \
  کرو                                                              \
    اگر (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (قدر_2);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        جاؤ yybackup;                                            \
      }                                                           \
    ورنہ                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  جب (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#تعریف_کرو YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#اگر YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(دلائل_2)                        \
کرو {                                            \
  اگر (yydebug)                                  \
    YYFPRINTF دلائل_2;                             \
} جب (0)




# define YY_علامت_PRINT(Title, قسم_2, قدر_2, Location)                    \
کرو {                                                                      \
  اگر (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_علامت_print (stderr,                                            \
                  قسم_2, قدر_2, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} جب (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

ساکن خالی
yy_علامت_قدر_print (FILE *yyo,
                       yysymbol_قسم_t yykind, YYSTYPE ثابت * ثابت yyvaluep, ساخت parser_param *param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  اگر (!yyvaluep)
    واپس;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_انتہا
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

ساکن خالی
yy_علامت_print (FILE *yyo,
                 yysymbol_قسم_t yykind, YYSTYPE ثابت * ثابت yyvaluep, ساخت parser_param *param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_علامت_قدر_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

ساکن خالی
yy_ڈھیر_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  برائے (; yybottom <= yytop; yybottom++)
    {
      صحیح yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_ڈھیر_PRINT(Bottom, چوٹی)                            \
کرو {                                                            \
  اگر (yydebug)                                                  \
    yy_ڈھیر_print ((Bottom), (چوٹی));                           \
} جب (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

ساکن خالی
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 صحیح yyrule, ساخت parser_param *param)
{
  صحیح yylno = yyrline[yyrule];
  صحیح yynrhs = yyr2[yyrule];
  صحیح yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  برائے (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_علامت_print (stderr,
                       YY_ACCESSING_علامت (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(قاعدہ)          \
کرو {                                    \
  اگر (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, قاعدہ, param); \
} جب (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
صحیح yydebug;
#ورنہ /* !YYDEBUG */
# define YYDPRINTF(دلائل_2) ((خالی) 0)
# define YY_علامت_PRINT(Title, قسم_2, قدر_2, Location)
# define YY_ڈھیر_PRINT(Bottom, چوٹی)
# define YY_REDUCE_PRINT(قاعدہ)
#اگر_ختم /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#اگر_غیر_تعریف_شدہ YYINITDEPTH
# define YYINITDEPTH 200
#اگر_ختم

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#اگر_غیر_تعریف_شدہ YYMAXDEPTH
# define YYMAXDEPTH 10000
#اگر_ختم


/* Context of a parse error.  */
قسمتعریف ساخت
{
  yy_state_t *yyssp;
  yysymbol_قسم_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
ساکن صحیح
yypcontext_expected_علامات (ثابت yypcontext_t *yyctx,
                            yysymbol_قسم_t yyarg[], صحیح yyargn)
{
  /* Actual size of YYARG. */
  صحیح yycount = 0;
  صحیح yyn = yypact[+*yyctx->yyssp];
  اگر (!yypact_قدر_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      صحیح yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      صحیح yychecklim = YYLAST - yyn + 1;
      صحیح yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      صحیح yyx;
      برائے (yyx = yyxbegin; yyx < yyxend; ++yyx)
        اگر (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_قدر_is_error (yytable[yyx + yyn]))
          {
            اگر (!yyarg)
              ++yycount;
            ورنہ اگر (yycount == yyargn)
              واپس 0;
            ورنہ
              yyarg[yycount++] = YY_CAST (yysymbol_قسم_t, yyx);
          }
    }
  اگر (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  واپس yycount;
}




#اگر_غیر_تعریف_شدہ yystrlen
# اگر defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, لڑی_لمبائی (S)))
# ورنہ
/* Return the length of YYSTR.  */
ساکن YYPTRDIFF_T
yystrlen (ثابت حرف *yystr)
{
  YYPTRDIFF_T yylen;
  برائے (yylen = 0; yystr[yylen]; yylen++)
    جاری;
  واپس yylen;
}
# endif
#اگر_ختم

#اگر_غیر_تعریف_شدہ yystpcpy
# اگر defined __GLIBC__ && defined _STRING_H && defined GNU_ماخذ
#  define yystpcpy stpcpy
# ورنہ
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
ساکن حرف *
yystpcpy (حرف *yydest, ثابت حرف *yysrc)
{
  حرف *yyd = yydest;
  ثابت حرف *yys = yysrc;

  جب ((*yyd++ = *yys++) != '\0')
    جاری;

  واپس yyd - 1;
}
# endif
#اگر_ختم



ساکن صحیح
yy_syntax_error_arguments (ثابت yypcontext_t *yyctx,
                           yysymbol_قسم_t yyarg[], صحیح yyargn)
{
  /* Actual size of YYARG. */
  صحیح yycount = 0;
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
  اگر (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      صحیح yyn;
      اگر (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_علامات (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      اگر (yyn == YYENOMEM)
        واپس YYENOMEM;
      ورنہ
        yycount += yyn;
    }
  واپس yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
ساکن صحیح
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, حرف **yymsg,
                ثابت yypcontext_t *yyctx)
{
  شمار { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  ثابت حرف *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_قسم_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  صحیح yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  اگر (yycount == YYENOMEM)
    واپس YYENOMEM;

  انتخاب (yycount)
    {
#تعریف_کرو YYCASE_(N, S)                       \
      حالت N:                               \
        yyformat = S;                       \
        توڑو
    طےشدہ: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#تعریف_منسوخ YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    صحیح yyi;
    برائے (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        اگر (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_زیادہ_سے_زیادہ)
          yysize = yysize1;
        ورنہ
          واپس YYENOMEM;
      }
  }

  اگر (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      اگر (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_زیادہ_سے_زیادہ))
        *yymsg_alloc = YYSTACK_ALLOC_زیادہ_سے_زیادہ;
      واپس -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    حرف *yyp = *yymsg;
    صحیح yyi = 0;
    جب ((*yyp = *yyformat) != '\0')
      اگر (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      ورنہ
        {
          ++yyp;
          ++yyformat;
        }
  }
  واپس 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

ساکن خالی
yydestruct (ثابت حرف *yymsg,
            yysymbol_قسم_t yykind, YYSTYPE *yyvaluep, ساخت parser_param *param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  اگر (!yymsg)
    yymsg = "Deleting";
  YY_علامت_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  انتخاب (yykind)
    {
    حالت YYSYMBOL_STRING: /* "string literal"  */
#سطر 259 "src/xkbcomp/parser.y"
            { آزاد(((*yyvaluep).str)); }
#سطر 1640 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_XkbFile: /* XkbFile  */
#سطر 257 "src/xkbcomp/parser.y"
            { اگر (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#سطر 1646 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Xkb_مرکب_Map: /* XkbCompositeMap  */
#سطر 257 "src/xkbcomp/parser.y"
            { اگر (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#سطر 1652 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_XkbMapConfigList: /* XkbMapConfigList  */
#سطر 258 "src/xkbcomp/parser.y"
            { FreeXkbFile(((*yyvaluep).fileList).سر); }
#سطر 1658 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_XkbMapConfig: /* XkbMapConfig  */
#سطر 257 "src/xkbcomp/parser.y"
            { اگر (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#سطر 1664 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_DeclList: /* DeclList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).anyList).سر); }
#سطر 1670 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Decl: /* Decl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).any)); }
#سطر 1676 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_VarDecl: /* VarDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#سطر 1682 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_کلید_Name_Decl: /* KeyNameDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).کلید_رمز)); }
#سطر 1688 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_کلید_Alias_Decl: /* KeyAliasDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).کلید_Alias)); }
#سطر 1694 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_VModDecl: /* VModDecl  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).سر); }
#سطر 1700 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_VModDefList: /* VModDefList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).سر); }
#سطر 1706 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_VModDef: /* VModDef  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmod)); }
#سطر 1712 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_InterpretDecl: /* InterpretDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#سطر 1718 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Interpret_مطابقت: /* InterpretMatch  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#سطر 1724 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_VarDeclList: /* VarDeclList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).سر); }
#سطر 1730 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_کلید_Type_Decl: /* KeyTypeDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).کلید_Type)); }
#سطر 1736 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_SymbolsDecl: /* SymbolsDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).syms)); }
#سطر 1742 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_OptSymbolsBody: /* OptSymbolsBody  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).سر); }
#سطر 1748 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_SymbolsBody: /* SymbolsBody  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).سر); }
#سطر 1754 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_SymbolsVarDecl: /* SymbolsVarDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#سطر 1760 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Multi_کلید_Sym_Or_Action_List: /* MultiKeySymOrActionList  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1766 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_گروہ_Compat_Decl: /* GroupCompatDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).گروہ_Compat)); }
#سطر 1772 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_ModMapDecl: /* ModMapDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).mod_نقاب_2)); }
#سطر 1778 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_کلید_Or_کلید_Sym_List: /* KeyOrKeySymList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ_List).سر); }
#سطر 1784 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_کلید_Or_کلید_Sym: /* KeyOrKeySym  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1790 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_LedMapDecl: /* LedMapDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledMap)); }
#سطر 1796 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_LedNameDecl: /* LedNameDecl  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledName)); }
#سطر 1802 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_CoordList: /* CoordList  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1808 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Coord: /* Coord  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1814 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_اظہاریہ_List: /* ExprList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ_List).سر); }
#سطر 1820 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_اظہاریہ: /*اظہاریہ*/
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1826 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_رکن: /*رکن*/
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1832 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_MultiActionList: /* MultiActionList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ_List).سر); }
#سطر 1838 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_ActionList: /* ActionList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ_List).سر); }
#سطر 1844 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Non_خالی_خانے_Actions: /* NonEmptyActions  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1850 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Actions: /* Actions  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1856 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Action: /* Action  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1862 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Lhs: /* Lhs  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1868 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_OptTerminal: /* OptTerminal  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1874 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Terminal: /* Terminal  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1880 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Multi_کلید_Sym_List: /* MultiKeySymList  */
#سطر 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ_List).سر); }
#سطر 1886 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_کلید_Sym_List: /* KeySymList  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1892 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_Non_خالی_خانے_کلید_Syms: /* NonEmptyKeySyms  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1898 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_کلید_Syms: /* KeySyms  */
#سطر 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).اظہاریہ)); }
#سطر 1904 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_OptMapName: /* OptMapName  */
#سطر 259 "src/xkbcomp/parser.y"
            { آزاد(((*yyvaluep).str)); }
#سطر 1910 "src/xkbcomp/parser.c"
        توڑو;

    حالت YYSYMBOL_MapName: /* MapName  */
#سطر 259 "src/xkbcomp/parser.y"
            { آزاد(((*yyvaluep).str)); }
#سطر 1916 "src/xkbcomp/parser.c"
        توڑو;

      طےشدہ:
        توڑو;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_انتہا
}






/*----------.
| yyparse.  |
`----------*/

صحیح
yyparse (ساخت parser_param *param)
{
/* Lookahead token kind.  */
صحیح yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_قدر (ساکن YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_قدر (= yyval_default);

    /* Number of syntax errors so far.  */
    صحیح yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    صحیح yyerrstatus = 0;

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

  صحیح yyn;
  /* The return value of yyparse.  */
  صحیح yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_قسم_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  حرف yymsgbuf[128];
  حرف *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = حجم yymsgbuf;

#تعریف_کرو YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  صحیح yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  جاؤ yysetstate;


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
  YY_IGNORE_USELESS_CAST_انتہا
  YY_ڈھیر_PRINT (yyss, yyssp);

  اگر (yyss + yystacksize - 1 <= yyssp)
#اگر !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#ورنہ
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# اگر defined yyoverflow
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
# ورنہ /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      اگر (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      اگر (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        اتحاد yyalloc *yyptr =
          YY_CAST (اتحاد yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_بائٹیں (yystacksize))));
        اگر (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        اگر (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (طویل, yystacksize)));
      YY_IGNORE_USELESS_CAST_انتہا

      اگر (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#اگر_ختم /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  اگر (yystate == YYFINAL)
    YYACCEPT;

  جاؤ yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  اگر (yypact_قدر_is_default (yyn))
    جاؤ yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  اگر (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, param_scanner);
    }

  اگر (yychar <= انتہا_کا_FILE)
    {
      yychar = انتہا_کا_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  ورنہ اگر (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      جاؤ yyerrlab1;
    }
  ورنہ
    {
      yytoken = YYTRANSLATE (yychar);
      YY_علامت_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  اگر (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    جاؤ yydefault;
  yyn = yytable[yyn];
  اگر (yyn <= 0)
    {
      اگر (yytable_قدر_is_error (yyn))
        جاؤ yyerrlab;
      yyn = -yyn;
      جاؤ yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  اگر (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_علامت_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_انتہا

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  جاؤ yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  اگر (yyn == 0)
    جاؤ yyerrlab;
  جاؤ yyreduce;


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
  انتخاب (yyn)
    {
  حالت 2: /* XkbFile: XkbCompositeMap  */
#سطر 276 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->مزید_maps = !!param->rtrn; (خالی) yynerrs; }
#سطر 2195 "src/xkbcomp/parser.c"
    توڑو;

  حالت 3: /* XkbFile: XkbMapConfig  */
#سطر 278 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->مزید_maps = !!param->rtrn; YYACCEPT; }
#سطر 2201 "src/xkbcomp/parser.c"
    توڑو;

  حالت 4: /* XkbFile: "end of file"  */
#سطر 280 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = تہی; param->مزید_maps = false; }
#سطر 2207 "src/xkbcomp/parser.c"
    توڑو;

  حالت 5: /* XkbCompositeMap: OptFlags XkbCompositeType OptMapName "{" XkbMapConfigList "}" ";"  */
#سطر 286 "src/xkbcomp/parser.y"
                        { (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (ParseCommon *) (yyvsp[-2].fileList).سر, (yyvsp[-6].mapFlags)); }
#سطر 2213 "src/xkbcomp/parser.c"
    توڑو;

  حالت 6: /* XkbCompositeType: "xkb_keymap"  */
#سطر 289 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#سطر 2219 "src/xkbcomp/parser.c"
    توڑو;

  حالت 7: /* XkbCompositeType: "xkb_semantics"  */
#سطر 290 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#سطر 2225 "src/xkbcomp/parser.c"
    توڑو;

  حالت 8: /* XkbCompositeType: "xkb_layout"  */
#سطر 291 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#سطر 2231 "src/xkbcomp/parser.c"
    توڑو;

  حالت 9: /* XkbMapConfigList: XkbMapConfigList XkbMapConfig  */
#سطر 298 "src/xkbcomp/parser.y"
                        {
                            اگر ((yyvsp[0].file)) {
                                اگر ((yyvsp[-1].fileList).سر) {
                                    (yyval.fileList).سر = (yyvsp[-1].fileList).سر;
                                    (yyval.fileList).آخری->common.اگلا = &(yyvsp[0].file)->common;
                                    (yyval.fileList).آخری = (yyvsp[0].file);
                                } ورنہ {
                                    (yyval.fileList).سر = (yyval.fileList).آخری = (yyvsp[0].file);
                                }
                            }
                        }
#سطر 2247 "src/xkbcomp/parser.c"
    توڑو;

  حالت 10: /* XkbMapConfigList: %empty  */
#سطر 309 "src/xkbcomp/parser.y"
                        { (yyval.fileList).سر = (yyval.fileList).آخری = تہی; }
#سطر 2253 "src/xkbcomp/parser.c"
    توڑو;

  حالت 11: /* XkbMapConfig: OptFlags FileType OptMapName "{" DeclList "}" ";"  */
#سطر 315 "src/xkbcomp/parser.y"
                        {
                            اگر ((yyvsp[-6].mapFlags) & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            محفوظ_map_name((yyvsp[-4].str)));
                            }
                            (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (yyvsp[-2].anyList).سر, (yyvsp[-6].mapFlags));
                        }
#سطر 2266 "src/xkbcomp/parser.c"
    توڑو;

  حالت 12: /* FileType: "xkb_keycodes"  */
#سطر 325 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_KEYCODES; }
#سطر 2272 "src/xkbcomp/parser.c"
    توڑو;

  حالت 13: /* FileType: "xkb_types"  */
#سطر 326 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_TYPES; }
#سطر 2278 "src/xkbcomp/parser.c"
    توڑو;

  حالت 14: /* FileType: "xkb_compatibility"  */
#سطر 327 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_COMPAT; }
#سطر 2284 "src/xkbcomp/parser.c"
    توڑو;

  حالت 15: /* FileType: "xkb_symbols"  */
#سطر 328 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_SYMBOLS; }
#سطر 2290 "src/xkbcomp/parser.c"
    توڑو;

  حالت 16: /* FileType: "xkb_geometry"  */
#سطر 329 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_GEOMETRY; }
#سطر 2296 "src/xkbcomp/parser.c"
    توڑو;

  حالت 17: /* OptFlags: Flags  */
#سطر 332 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#سطر 2302 "src/xkbcomp/parser.c"
    توڑو;

  حالت 18: /* OptFlags: %empty  */
#سطر 333 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = 0; }
#سطر 2308 "src/xkbcomp/parser.c"
    توڑو;

  حالت 19: /* Flags: Flags Flag  */
#سطر 336 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = ((yyvsp[-1].mapFlags) | (yyvsp[0].mapFlags)); }
#سطر 2314 "src/xkbcomp/parser.c"
    توڑو;

  حالت 20: /* Flags: Flag  */
#سطر 337 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#سطر 2320 "src/xkbcomp/parser.c"
    توڑو;

  حالت 21: /* Flag: "partial"  */
#سطر 340 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_PARTIAL; }
#سطر 2326 "src/xkbcomp/parser.c"
    توڑو;

  حالت 22: /* Flag: "default"  */
#سطر 341 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEFAULT; }
#سطر 2332 "src/xkbcomp/parser.c"
    توڑو;

  حالت 23: /* Flag: "hidden"  */
#سطر 342 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_HIDDEN; }
#سطر 2338 "src/xkbcomp/parser.c"
    توڑو;

  حالت 24: /* Flag: "alphanumeric_keys"  */
#سطر 343 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_ALPHANUMERIC; }
#سطر 2344 "src/xkbcomp/parser.c"
    توڑو;

  حالت 25: /* Flag: "modifier_keys"  */
#سطر 344 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_MODIFIER; }
#سطر 2350 "src/xkbcomp/parser.c"
    توڑو;

  حالت 26: /* Flag: "keypad_keys"  */
#سطر 345 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_KEYPAD; }
#سطر 2356 "src/xkbcomp/parser.c"
    توڑو;

  حالت 27: /* Flag: "function_keys"  */
#سطر 346 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_FN; }
#سطر 2362 "src/xkbcomp/parser.c"
    توڑو;

  حالت 28: /* Flag: "alternate_group"  */
#سطر 347 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_ALTGR; }
#سطر 2368 "src/xkbcomp/parser.c"
    توڑو;

  حالت 29: /* Flag: "deprecated"  */
#سطر 348 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEPRECATED; }
#سطر 2374 "src/xkbcomp/parser.c"
    توڑو;

  حالت 30: /* Flag: "identifier"  */
#سطر 350 "src/xkbcomp/parser.y"
                        {
                            ثابت bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_رمز(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_اقل,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (غیرعلامتی)(yyvsp[0].sval).len, (yyvsp[0].sval).آغاز,
                                (error ? "" : "; ignored")
                            );
                            اگر (error)
                                YYABORT;
                            (yyval.mapFlags) = 0;
                        }
#سطر 2393 "src/xkbcomp/parser.c"
    توڑو;

  حالت 31: /* DeclList: DeclList Decl  */
#سطر 367 "src/xkbcomp/parser.y"
                        {
                            اگر ((yyvsp[0].any)) {
                                اگر ((yyvsp[-1].anyList).سر) {
                                    (yyval.anyList).سر = (yyvsp[-1].anyList).سر; (yyvsp[-1].anyList).آخری->اگلا = (yyvsp[0].any); (yyval.anyList).آخری = (yyvsp[0].any);
                                } ورنہ {
                                    (yyval.anyList).سر = (yyval.anyList).آخری = (yyvsp[0].any);
                                }
                            }
                        }
#سطر 2407 "src/xkbcomp/parser.c"
    توڑو;

  حالت 32: /* DeclList: DeclList OptMergeMode VModDecl  */
#سطر 382 "src/xkbcomp/parser.y"
                        {
                            برائے (VModDef *vmod = (yyvsp[0].vmodList).سر; vmod; vmod = (VModDef *) vmod->common.اگلا)
                                vmod->ملاؤ = (yyvsp[-1].ملاؤ);
                            اگر ((yyvsp[-2].anyList).سر) {
                                (yyval.anyList).سر = (yyvsp[-2].anyList).سر; (yyvsp[-2].anyList).آخری->اگلا = &(yyvsp[0].vmodList).سر->common; (yyval.anyList).آخری = &(yyvsp[0].vmodList).آخری->common;
                            } ورنہ {
                                (yyval.anyList).سر = &(yyvsp[0].vmodList).سر->common; (yyval.anyList).آخری = &(yyvsp[0].vmodList).آخری->common;
                            }
                        }
#سطر 2421 "src/xkbcomp/parser.c"
    توڑو;

  حالت 33: /* DeclList: %empty  */
#سطر 391 "src/xkbcomp/parser.y"
                        { (yyval.anyList).سر = (yyval.anyList).آخری = تہی; }
#سطر 2427 "src/xkbcomp/parser.c"
    توڑو;

  حالت 34: /* Decl: OptMergeMode VarDecl  */
#سطر 395 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].var)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].var);
                        }
#سطر 2436 "src/xkbcomp/parser.c"
    توڑو;

  حالت 35: /* Decl: OptMergeMode InterpretDecl  */
#سطر 401 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].interp)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].interp);
                        }
#سطر 2445 "src/xkbcomp/parser.c"
    توڑو;

  حالت 36: /* Decl: OptMergeMode KeyNameDecl  */
#سطر 406 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].کلید_رمز)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].کلید_رمز);
                        }
#سطر 2454 "src/xkbcomp/parser.c"
    توڑو;

  حالت 37: /* Decl: OptMergeMode KeyAliasDecl  */
#سطر 411 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].کلید_Alias)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].کلید_Alias);
                        }
#سطر 2463 "src/xkbcomp/parser.c"
    توڑو;

  حالت 38: /* Decl: OptMergeMode KeyTypeDecl  */
#سطر 416 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].کلید_Type)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].کلید_Type);
                        }
#سطر 2472 "src/xkbcomp/parser.c"
    توڑو;

  حالت 39: /* Decl: OptMergeMode SymbolsDecl  */
#سطر 421 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].syms)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].syms);
                        }
#سطر 2481 "src/xkbcomp/parser.c"
    توڑو;

  حالت 40: /* Decl: OptMergeMode ModMapDecl  */
#سطر 426 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].mod_نقاب_2)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].mod_نقاب_2);
                        }
#سطر 2490 "src/xkbcomp/parser.c"
    توڑو;

  حالت 41: /* Decl: OptMergeMode GroupCompatDecl  */
#سطر 431 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].گروہ_Compat)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].گروہ_Compat);
                        }
#سطر 2499 "src/xkbcomp/parser.c"
    توڑو;

  حالت 42: /* Decl: OptMergeMode LedMapDecl  */
#سطر 436 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledMap)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledMap);
                        }
#سطر 2508 "src/xkbcomp/parser.c"
    توڑو;

  حالت 43: /* Decl: OptMergeMode LedNameDecl  */
#سطر 441 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledName)->ملاؤ = (yyvsp[-1].ملاؤ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledName);
                        }
#سطر 2517 "src/xkbcomp/parser.c"
    توڑو;

  حالت 44: /* Decl: OptMergeMode ShapeDecl  */
#سطر 445 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = تہی; }
#سطر 2523 "src/xkbcomp/parser.c"
    توڑو;

  حالت 45: /* Decl: OptMergeMode SectionDecl  */
#سطر 446 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = تہی; }
#سطر 2529 "src/xkbcomp/parser.c"
    توڑو;

  حالت 46: /* Decl: OptMergeMode DoodadDecl  */
#سطر 447 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = تہی; }
#سطر 2535 "src/xkbcomp/parser.c"
    توڑو;

  حالت 47: /* Decl: OptMergeMode UnknownDecl  */
#سطر 449 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#سطر 2541 "src/xkbcomp/parser.c"
    توڑو;

  حالت 48: /* Decl: OptMergeMode UnknownCompoundStatementDecl  */
#سطر 451 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#سطر 2547 "src/xkbcomp/parser.c"
    توڑو;

  حالت 49: /* Decl: MergeMode "string literal"  */
#سطر 453 "src/xkbcomp/parser.y"
                        {
                            (yyval.any) = (ParseCommon *) IncludeCreate(param->ctx, (yyvsp[0].str), (yyvsp[-1].ملاؤ));
                            آزاد((yyvsp[0].str));
                        }
#سطر 2556 "src/xkbcomp/parser.c"
    توڑو;

  حالت 50: /* VarDecl: Lhs "=" Expr ";"  */
#سطر 460 "src/xkbcomp/parser.y"
                        { (yyval.var) = VarCreate((yyvsp[-3].اظہاریہ), (yyvsp[-1].اظہاریہ)); }
#سطر 2562 "src/xkbcomp/parser.c"
    توڑو;

  حالت 51: /* VarDecl: Ident ";"  */
#سطر 462 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), true); }
#سطر 2568 "src/xkbcomp/parser.c"
    توڑو;

  حالت 52: /* VarDecl: "!" Ident ";"  */
#سطر 464 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), false); }
#سطر 2574 "src/xkbcomp/parser.c"
    توڑو;

  حالت 53: /* KeyNameDecl: "key name" "=" KeyCode ";"  */
#سطر 468 "src/xkbcomp/parser.y"
                        { (yyval.کلید_رمز) = KeycodeCreate((yyvsp[-3].atom), (yyvsp[-1].عدد)); }
#سطر 2580 "src/xkbcomp/parser.c"
    توڑو;

  حالت 54: /* KeyAliasDecl: "alias" "key name" "=" "key name" ";"  */
#سطر 472 "src/xkbcomp/parser.y"
                        { (yyval.کلید_Alias) = کلید_Alias_Create((yyvsp[-3].atom), (yyvsp[-1].atom)); }
#سطر 2586 "src/xkbcomp/parser.c"
    توڑو;

  حالت 55: /* VModDecl: "virtual_modifiers" VModDefList ";"  */
#سطر 476 "src/xkbcomp/parser.y"
                        { (yyval.vmodList) = (yyvsp[-1].vmodList); }
#سطر 2592 "src/xkbcomp/parser.c"
    توڑو;

  حالت 56: /* VModDefList: VModDefList "," VModDef  */
#سطر 480 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).سر = (yyvsp[-2].vmodList).سر; (yyval.vmodList).آخری->common.اگلا = &(yyvsp[0].vmod)->common; (yyval.vmodList).آخری = (yyvsp[0].vmod); }
#سطر 2598 "src/xkbcomp/parser.c"
    توڑو;

  حالت 57: /* VModDefList: VModDef  */
#سطر 482 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).سر = (yyval.vmodList).آخری = (yyvsp[0].vmod); }
#سطر 2604 "src/xkbcomp/parser.c"
    توڑو;

  حالت 58: /* VModDef: Ident  */
#سطر 486 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[0].atom), تہی); }
#سطر 2610 "src/xkbcomp/parser.c"
    توڑو;

  حالت 59: /* VModDef: Ident "=" Expr  */
#سطر 488 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[-2].atom), (yyvsp[0].اظہاریہ)); }
#سطر 2616 "src/xkbcomp/parser.c"
    توڑو;

  حالت 60: /* InterpretDecl: "interpret" InterpretMatch "{" VarDeclList "}" ";"  */
#سطر 494 "src/xkbcomp/parser.y"
                        { (yyvsp[-4].interp)->def = (yyvsp[-2].varList).سر; (yyval.interp) = (yyvsp[-4].interp); }
#سطر 2622 "src/xkbcomp/parser.c"
    توڑو;

  حالت 61: /* InterpretMatch: KeySym "+" Expr  */
#سطر 498 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[-2].keysym), (yyvsp[0].اظہاریہ)); }
#سطر 2628 "src/xkbcomp/parser.c"
    توڑو;

  حالت 62: /* InterpretMatch: KeySym  */
#سطر 500 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[0].keysym), تہی); }
#سطر 2634 "src/xkbcomp/parser.c"
    توڑو;

  حالت 63: /* VarDeclList: VarDeclList VarDecl  */
#سطر 504 "src/xkbcomp/parser.y"
                        {
                            اگر ((yyvsp[0].var)) {
                                اگر ((yyvsp[-1].varList).سر) {
                                    (yyval.varList).سر = (yyvsp[-1].varList).سر;
                                    (yyval.varList).آخری->common.اگلا = &(yyvsp[0].var)->common;
                                    (yyval.varList).آخری = (yyvsp[0].var);
                                } ورنہ {
                                    (yyval.varList).سر = (yyval.varList).آخری = (yyvsp[0].var);
                                }
                            }
                        }
#سطر 2650 "src/xkbcomp/parser.c"
    توڑو;

  حالت 64: /* VarDeclList: %empty  */
#سطر 515 "src/xkbcomp/parser.y"
                        { (yyval.varList).سر = (yyval.varList).آخری = تہی; }
#سطر 2656 "src/xkbcomp/parser.c"
    توڑو;

  حالت 65: /* KeyTypeDecl: "type" String "{" VarDeclList "}" ";"  */
#سطر 521 "src/xkbcomp/parser.y"
                        { (yyval.کلید_Type) = کلید_Type_Create((yyvsp[-4].atom), (yyvsp[-2].varList).سر); }
#سطر 2662 "src/xkbcomp/parser.c"
    توڑو;

  حالت 66: /* SymbolsDecl: "key" "key name" "{" OptSymbolsBody "}" ";"  */
#سطر 527 "src/xkbcomp/parser.y"
                        { (yyval.syms) = SymbolsCreate((yyvsp[-4].atom), (yyvsp[-2].varList).سر); }
#سطر 2668 "src/xkbcomp/parser.c"
    توڑو;

  حالت 67: /* OptSymbolsBody: SymbolsBody  */
#سطر 530 "src/xkbcomp/parser.y"
                                    { (yyval.varList) = (yyvsp[0].varList); }
#سطر 2674 "src/xkbcomp/parser.c"
    توڑو;

  حالت 68: /* OptSymbolsBody: %empty  */
#سطر 531 "src/xkbcomp/parser.y"
                                    { (yyval.varList).سر = (yyval.varList).آخری = تہی; }
#سطر 2680 "src/xkbcomp/parser.c"
    توڑو;

  حالت 69: /* SymbolsBody: SymbolsBody "," SymbolsVarDecl  */
#سطر 535 "src/xkbcomp/parser.y"
                        { (yyval.varList).سر = (yyvsp[-2].varList).سر; (yyval.varList).آخری->common.اگلا = &(yyvsp[0].var)->common; (yyval.varList).آخری = (yyvsp[0].var); }
#سطر 2686 "src/xkbcomp/parser.c"
    توڑو;

  حالت 70: /* SymbolsBody: SymbolsVarDecl  */
#سطر 537 "src/xkbcomp/parser.y"
                        { (yyval.varList).سر = (yyval.varList).آخری = (yyvsp[0].var); }
#سطر 2692 "src/xkbcomp/parser.c"
    توڑو;

  حالت 71: /* SymbolsVarDecl: Lhs "=" Expr  */
#سطر 540 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate((yyvsp[-2].اظہاریہ), (yyvsp[0].اظہاریہ)); }
#سطر 2698 "src/xkbcomp/parser.c"
    توڑو;

  حالت 72: /* SymbolsVarDecl: Lhs "=" MultiKeySymOrActionList  */
#سطر 541 "src/xkbcomp/parser.y"
                                                           { (yyval.var) = VarCreate((yyvsp[-2].اظہاریہ), (yyvsp[0].اظہاریہ)); }
#سطر 2704 "src/xkbcomp/parser.c"
    توڑو;

  حالت 73: /* SymbolsVarDecl: Ident  */
#سطر 542 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), true); }
#سطر 2710 "src/xkbcomp/parser.c"
    توڑو;

  حالت 74: /* SymbolsVarDecl: "!" Ident  */
#سطر 543 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), false); }
#سطر 2716 "src/xkbcomp/parser.c"
    توڑو;

  حالت 75: /* SymbolsVarDecl: MultiKeySymOrActionList  */
#سطر 544 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate(تہی, (yyvsp[0].اظہاریہ)); }
#سطر 2722 "src/xkbcomp/parser.c"
    توڑو;

  حالت 76: /* MultiKeySymOrActionList: "[" MultiKeySymList "]"  */
#سطر 560 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[-1].اظہاریہ_List).سر; }
#سطر 2728 "src/xkbcomp/parser.c"
    توڑو;

  حالت 77: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiKeySymList "]"  */
#سطر 562 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoSymbol */
                            ساخت {اظہاریہ_Def *سر; اظہاریہ_Def *آخری;} list = {
                                .سر = (yyvsp[-1].اظہاریہ_List).سر, .آخری = (yyvsp[-1].اظہاریہ_List).آخری
                            };
                            برائے (uint32_t k = 0; k < (yyvsp[-3].no_علامت_Or_Action_List); k++) {
                                اظہاریہ_Def* ثابت syms =
                                    اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت);
                                اگر (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.اگلا = &list.سر->common;
                                list.سر = syms;
                            }
                            (yyval.اظہاریہ) = list.سر;
                        }
#سطر 2750 "src/xkbcomp/parser.c"
    توڑو;

  حالت 78: /* MultiKeySymOrActionList: "[" MultiActionList "]"  */
#سطر 580 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[-1].اظہاریہ_List).سر; }
#سطر 2756 "src/xkbcomp/parser.c"
    توڑو;

  حالت 79: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiActionList "]"  */
#سطر 582 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoAction() */
                            ساخت {اظہاریہ_Def *سر; اظہاریہ_Def *آخری;} list = {
                                .سر = (yyvsp[-1].اظہاریہ_List).سر, .آخری = (yyvsp[-1].اظہاریہ_List).آخری
                            };
                            برائے (uint32_t k = 0; k < (yyvsp[-3].no_علامت_Or_Action_List); k++) {
                                اظہاریہ_Def* ثابت acts = اظہاریہ_Create_Action_List(تہی);
                                اگر (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.اگلا = &list.سر->common;
                                list.سر = acts;
                            }
                            (yyval.اظہاریہ) = list.سر;
                        }
#سطر 2777 "src/xkbcomp/parser.c"
    توڑو;

  حالت 80: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "]"  */
#سطر 604 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_خالی_خانے_List(); }
#سطر 2783 "src/xkbcomp/parser.c"
    توڑو;

  حالت 81: /* NoSymbolOrActionList: NoSymbolOrActionList "," "{" "}"  */
#سطر 610 "src/xkbcomp/parser.y"
                        { (yyval.no_علامت_Or_Action_List) = (yyvsp[-3].no_علامت_Or_Action_List) + 1; }
#سطر 2789 "src/xkbcomp/parser.c"
    توڑو;

  حالت 82: /* NoSymbolOrActionList: "{" "}"  */
#سطر 612 "src/xkbcomp/parser.y"
                        { (yyval.no_علامت_Or_Action_List) = 1; }
#سطر 2795 "src/xkbcomp/parser.c"
    توڑو;

  حالت 83: /* NoSymbolOrActionList: %empty  */
#سطر 613 "src/xkbcomp/parser.y"
                        { (yyval.no_علامت_Or_Action_List) = 0; }
#سطر 2801 "src/xkbcomp/parser.c"
    توڑو;

  حالت 84: /* GroupCompatDecl: "group" Integer "=" Expr ";"  */
#سطر 617 "src/xkbcomp/parser.y"
                        { (yyval.گروہ_Compat) = گروہ_Compat_Create((yyvsp[-3].عدد), (yyvsp[-1].اظہاریہ)); }
#سطر 2807 "src/xkbcomp/parser.c"
    توڑو;

  حالت 85: /* ModMapDecl: "modifier_map" Expr "{" KeyOrKeySymList "}" ";"  */
#سطر 621 "src/xkbcomp/parser.y"
                        {
                            اگر (param->config.format == XKB_KEYMAP_FORMAT_متن_V1 &&
                                (yyvsp[-4].اظہاریہ)->common.type != STMT_اظہاریہ_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_نقاب,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) (yyvsp[-4].اظہاریہ));
                                    FreeStmt((ParseCommon *) (yyvsp[-2].اظہاریہ_List).سر);
                                    YYERROR;
                            }
                            (yyval.mod_نقاب_2) = ModMapCreate((yyvsp[-4].اظہاریہ), (yyvsp[-2].اظہاریہ_List).سر);
                        }
#سطر 2826 "src/xkbcomp/parser.c"
    توڑو;

  حالت 86: /* KeyOrKeySymList: KeyOrKeySymList "," KeyOrKeySym  */
#سطر 638 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyvsp[-2].اظہاریہ_List).سر; (yyval.اظہاریہ_List).آخری->common.اگلا = &(yyvsp[0].اظہاریہ)->common; (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 2832 "src/xkbcomp/parser.c"
    توڑو;

  حالت 87: /* KeyOrKeySymList: KeyOrKeySym  */
#سطر 640 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 2838 "src/xkbcomp/parser.c"
    توڑو;

  حالت 88: /* KeyOrKeySym: "key name"  */
#سطر 644 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_کلید_Name((yyvsp[0].atom)); }
#سطر 2844 "src/xkbcomp/parser.c"
    توڑو;

  حالت 89: /* KeyOrKeySym: KeySym  */
#سطر 646 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_کلید_Sym((yyvsp[0].keysym)); }
#سطر 2850 "src/xkbcomp/parser.c"
    توڑو;

  حالت 90: /* LedMapDecl: "indicator" String "{" VarDeclList "}" ";"  */
#سطر 650 "src/xkbcomp/parser.y"
                        { (yyval.ledMap) = LedMapCreate((yyvsp[-4].atom), (yyvsp[-2].varList).سر); }
#سطر 2856 "src/xkbcomp/parser.c"
    توڑو;

  حالت 91: /* LedNameDecl: "indicator" Integer "=" Expr ";"  */
#سطر 654 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].عدد), (yyvsp[-1].اظہاریہ), false); }
#سطر 2862 "src/xkbcomp/parser.c"
    توڑو;

  حالت 92: /* LedNameDecl: "virtual" "indicator" Integer "=" Expr ";"  */
#سطر 656 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].عدد), (yyvsp[-1].اظہاریہ), true); }
#سطر 2868 "src/xkbcomp/parser.c"
    توڑو;

  حالت 93: /* UnknownDecl: "identifier" Terminal "=" Expr ";"  */
#سطر 660 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-3].اظہاریہ));
                            FreeStmt((ParseCommon *) (yyvsp[-1].اظہاریہ));
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, (yyvsp[-4].sval));
                        }
#سطر 2878 "src/xkbcomp/parser.c"
    توڑو;

  حالت 94: /* UnknownCompoundStatementDecl: "identifier" OptTerminal "{" VarDeclList "}" ";"  */
#سطر 669 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-4].اظہاریہ));
                            FreeStmt((ParseCommon *) (yyvsp[-2].varList).سر);
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, (yyvsp[-5].sval));
                        }
#سطر 2888 "src/xkbcomp/parser.c"
    توڑو;

  حالت 95: /* ShapeDecl: "shape" String "{" OutlineList "}" ";"  */
#سطر 677 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 2894 "src/xkbcomp/parser.c"
    توڑو;

  حالت 96: /* ShapeDecl: "shape" String "{" CoordList "}" ";"  */
#سطر 679 "src/xkbcomp/parser.y"
                        { (خالی) (yyvsp[-2].اظہاریہ); (yyval.geom) = تہی; }
#سطر 2900 "src/xkbcomp/parser.c"
    توڑو;

  حالت 97: /* SectionDecl: "section" String "{" SectionBody "}" ";"  */
#سطر 683 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 2906 "src/xkbcomp/parser.c"
    توڑو;

  حالت 98: /* SectionBody: SectionBody SectionBodyItem  */
#سطر 686 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = تہی;}
#سطر 2912 "src/xkbcomp/parser.c"
    توڑو;

  حالت 99: /* SectionBody: SectionBodyItem  */
#سطر 687 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = تہی; }
#سطر 2918 "src/xkbcomp/parser.c"
    توڑو;

  حالت 100: /* SectionBodyItem: "row" "{" RowBody "}" ";"  */
#سطر 691 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 2924 "src/xkbcomp/parser.c"
    توڑو;

  حالت 101: /* SectionBodyItem: VarDecl  */
#سطر 693 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = تہی; }
#سطر 2930 "src/xkbcomp/parser.c"
    توڑو;

  حالت 102: /* SectionBodyItem: DoodadDecl  */
#سطر 695 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 2936 "src/xkbcomp/parser.c"
    توڑو;

  حالت 103: /* SectionBodyItem: LedMapDecl  */
#سطر 697 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ledMap)); (yyval.geom) = تہی; }
#سطر 2942 "src/xkbcomp/parser.c"
    توڑو;

  حالت 104: /* SectionBodyItem: OverlayDecl  */
#سطر 699 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 2948 "src/xkbcomp/parser.c"
    توڑو;

  حالت 105: /* RowBody: RowBody RowBodyItem  */
#سطر 702 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = تہی;}
#سطر 2954 "src/xkbcomp/parser.c"
    توڑو;

  حالت 106: /* RowBody: RowBodyItem  */
#سطر 703 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = تہی; }
#سطر 2960 "src/xkbcomp/parser.c"
    توڑو;

  حالت 107: /* RowBodyItem: "keys" "{" Keys "}" ";"  */
#سطر 706 "src/xkbcomp/parser.y"
                                                     { (yyval.geom) = تہی; }
#سطر 2966 "src/xkbcomp/parser.c"
    توڑو;

  حالت 108: /* RowBodyItem: VarDecl  */
#سطر 708 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = تہی; }
#سطر 2972 "src/xkbcomp/parser.c"
    توڑو;

  حالت 109: /* Keys: Keys "," Key  */
#سطر 711 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = تہی; }
#سطر 2978 "src/xkbcomp/parser.c"
    توڑو;

  حالت 110: /* Keys: Key  */
#سطر 712 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = تہی; }
#سطر 2984 "src/xkbcomp/parser.c"
    توڑو;

  حالت 111: /* Key: "key name"  */
#سطر 716 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 2990 "src/xkbcomp/parser.c"
    توڑو;

  حالت 112: /* Key: "{" ExprList "}"  */
#سطر 718 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-1].اظہاریہ_List).سر); (yyval.geom) = تہی; }
#سطر 2996 "src/xkbcomp/parser.c"
    توڑو;

  حالت 113: /* OverlayDecl: "overlay" String "{" OverlayKeyList "}" ";"  */
#سطر 722 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 3002 "src/xkbcomp/parser.c"
    توڑو;

  حالت 114: /* OverlayKeyList: OverlayKeyList "," OverlayKey  */
#سطر 725 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = تہی; }
#سطر 3008 "src/xkbcomp/parser.c"
    توڑو;

  حالت 115: /* OverlayKeyList: OverlayKey  */
#سطر 726 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = تہی; }
#سطر 3014 "src/xkbcomp/parser.c"
    توڑو;

  حالت 116: /* OverlayKey: "key name" "=" "key name"  */
#سطر 729 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = تہی; }
#سطر 3020 "src/xkbcomp/parser.c"
    توڑو;

  حالت 117: /* OutlineList: OutlineList "," OutlineInList  */
#سطر 733 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی;}
#سطر 3026 "src/xkbcomp/parser.c"
    توڑو;

  حالت 118: /* OutlineList: OutlineInList  */
#سطر 735 "src/xkbcomp/parser.y"
                        { (yyval.geom) = تہی; }
#سطر 3032 "src/xkbcomp/parser.c"
    توڑو;

  حالت 119: /* OutlineInList: "{" CoordList "}"  */
#سطر 739 "src/xkbcomp/parser.y"
                        { (خالی) (yyvsp[-1].اظہاریہ); (yyval.geom) = تہی; }
#سطر 3038 "src/xkbcomp/parser.c"
    توڑو;

  حالت 120: /* OutlineInList: Ident "=" "{" CoordList "}"  */
#سطر 741 "src/xkbcomp/parser.y"
                        { (خالی) (yyvsp[-1].اظہاریہ); (yyval.geom) = تہی; }
#سطر 3044 "src/xkbcomp/parser.c"
    توڑو;

  حالت 121: /* OutlineInList: Ident "=" Expr  */
#سطر 743 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].اظہاریہ)); (yyval.geom) = تہی; }
#سطر 3050 "src/xkbcomp/parser.c"
    توڑو;

  حالت 122: /* CoordList: CoordList "," Coord  */
#سطر 747 "src/xkbcomp/parser.y"
                        { (خالی) (yyvsp[-2].اظہاریہ); (خالی) (yyvsp[0].اظہاریہ); (yyval.اظہاریہ) = تہی; }
#سطر 3056 "src/xkbcomp/parser.c"
    توڑو;

  حالت 123: /* CoordList: Coord  */
#سطر 749 "src/xkbcomp/parser.y"
                        { (خالی) (yyvsp[0].اظہاریہ); (yyval.اظہاریہ) = تہی; }
#سطر 3062 "src/xkbcomp/parser.c"
    توڑو;

  حالت 124: /* Coord: "[" SignedNumber "," SignedNumber "]"  */
#سطر 753 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = تہی; }
#سطر 3068 "src/xkbcomp/parser.c"
    توڑو;

  حالت 125: /* DoodadDecl: DoodadType String "{" VarDeclList "}" ";"  */
#سطر 757 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-2].varList).سر); (yyval.geom) = تہی; }
#سطر 3074 "src/xkbcomp/parser.c"
    توڑو;

  حالت 126: /* DoodadType: "text"  */
#سطر 760 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#سطر 3080 "src/xkbcomp/parser.c"
    توڑو;

  حالت 127: /* DoodadType: "outline"  */
#سطر 761 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#سطر 3086 "src/xkbcomp/parser.c"
    توڑو;

  حالت 128: /* DoodadType: "solid"  */
#سطر 762 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#سطر 3092 "src/xkbcomp/parser.c"
    توڑو;

  حالت 129: /* DoodadType: "logo"  */
#سطر 763 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#سطر 3098 "src/xkbcomp/parser.c"
    توڑو;

  حالت 130: /* FieldSpec: Ident  */
#سطر 766 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#سطر 3104 "src/xkbcomp/parser.c"
    توڑو;

  حالت 131: /* FieldSpec: Element  */
#سطر 767 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#سطر 3110 "src/xkbcomp/parser.c"
    توڑو;

  حالت 132: /* Element: "action"  */
#سطر 771 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "action"); }
#سطر 3116 "src/xkbcomp/parser.c"
    توڑو;

  حالت 133: /* Element: "interpret"  */
#سطر 773 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "interpret"); }
#سطر 3122 "src/xkbcomp/parser.c"
    توڑو;

  حالت 134: /* Element: "type"  */
#سطر 775 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "type"); }
#سطر 3128 "src/xkbcomp/parser.c"
    توڑو;

  حالت 135: /* Element: "key"  */
#سطر 777 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "key"); }
#سطر 3134 "src/xkbcomp/parser.c"
    توڑو;

  حالت 136: /* Element: "group"  */
#سطر 779 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "group"); }
#سطر 3140 "src/xkbcomp/parser.c"
    توڑو;

  حالت 137: /* Element: "modifier_map"  */
#سطر 781 "src/xkbcomp/parser.y"
                        {(yyval.atom) = xkb_atom_intern_literal(param->ctx, "modifier_map");}
#سطر 3146 "src/xkbcomp/parser.c"
    توڑو;

  حالت 138: /* Element: "indicator"  */
#سطر 783 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "indicator"); }
#سطر 3152 "src/xkbcomp/parser.c"
    توڑو;

  حالت 139: /* Element: "shape"  */
#سطر 785 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "shape"); }
#سطر 3158 "src/xkbcomp/parser.c"
    توڑو;

  حالت 140: /* Element: "row"  */
#سطر 787 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "row"); }
#سطر 3164 "src/xkbcomp/parser.c"
    توڑو;

  حالت 141: /* Element: "section"  */
#سطر 789 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "section"); }
#سطر 3170 "src/xkbcomp/parser.c"
    توڑو;

  حالت 142: /* Element: "text"  */
#سطر 791 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "text"); }
#سطر 3176 "src/xkbcomp/parser.c"
    توڑو;

  حالت 143: /* OptMergeMode: MergeMode  */
#سطر 794 "src/xkbcomp/parser.y"
                                        { (yyval.ملاؤ) = (yyvsp[0].ملاؤ); }
#سطر 3182 "src/xkbcomp/parser.c"
    توڑو;

  حالت 144: /* OptMergeMode: %empty  */
#سطر 795 "src/xkbcomp/parser.y"
                                        { (yyval.ملاؤ) = ملاؤ_DEFAULT; }
#سطر 3188 "src/xkbcomp/parser.c"
    توڑو;

  حالت 145: /* MergeMode: "include"  */
#سطر 798 "src/xkbcomp/parser.y"
                                        { (yyval.ملاؤ) = ملاؤ_DEFAULT; }
#سطر 3194 "src/xkbcomp/parser.c"
    توڑو;

  حالت 146: /* MergeMode: "augment"  */
#سطر 799 "src/xkbcomp/parser.y"
                                        { (yyval.ملاؤ) = ملاؤ_AUGMENT; }
#سطر 3200 "src/xkbcomp/parser.c"
    توڑو;

  حالت 147: /* MergeMode: "override"  */
#سطر 800 "src/xkbcomp/parser.y"
                                        { (yyval.ملاؤ) = ملاؤ_OVERRIDE; }
#سطر 3206 "src/xkbcomp/parser.c"
    توڑو;

  حالت 148: /* MergeMode: "replace"  */
#سطر 801 "src/xkbcomp/parser.y"
                                        { (yyval.ملاؤ) = ملاؤ_REPLACE; }
#سطر 3212 "src/xkbcomp/parser.c"
    توڑو;

  حالت 149: /* MergeMode: "alternate"  */
#سطر 803 "src/xkbcomp/parser.y"
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_پیغام_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    (yyval.ملاؤ) = ملاؤ_DEFAULT;
                }
#سطر 3226 "src/xkbcomp/parser.c"
    توڑو;

  حالت 150: /* ExprList: ExprList "," Expr  */
#سطر 815 "src/xkbcomp/parser.y"
                        {
                            اگر ((yyvsp[0].اظہاریہ)) {
                                اگر ((yyvsp[-2].اظہاریہ_List).سر) {
                                    (yyval.اظہاریہ_List).سر = (yyvsp[-2].اظہاریہ_List).سر;
                                    (yyval.اظہاریہ_List).آخری->common.اگلا = &(yyvsp[0].اظہاریہ)->common;
                                    (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ);
                                } ورنہ {
                                    (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ);
                                }
                            }
                        }
#سطر 3242 "src/xkbcomp/parser.c"
    توڑو;

  حالت 151: /* ExprList: Expr  */
#سطر 827 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 3248 "src/xkbcomp/parser.c"
    توڑو;

  حالت 152: /* ExprList: %empty  */
#سطر 828 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = تہی; }
#سطر 3254 "src/xkbcomp/parser.c"
    توڑو;

  حالت 153: /* Expr: Expr "/" Expr  */
#سطر 832 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_DIVIDE, (yyvsp[-2].اظہاریہ), (yyvsp[0].اظہاریہ)); }
#سطر 3260 "src/xkbcomp/parser.c"
    توڑو;

  حالت 154: /* Expr: Expr "+" Expr  */
#سطر 834 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_جوڑو, (yyvsp[-2].اظہاریہ), (yyvsp[0].اظہاریہ)); }
#سطر 3266 "src/xkbcomp/parser.c"
    توڑو;

  حالت 155: /* Expr: Expr "-" Expr  */
#سطر 836 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_تفریق, (yyvsp[-2].اظہاریہ), (yyvsp[0].اظہاریہ)); }
#سطر 3272 "src/xkbcomp/parser.c"
    توڑو;

  حالت 156: /* Expr: Expr "*" Expr  */
#سطر 838 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_MULTIPLY, (yyvsp[-2].اظہاریہ), (yyvsp[0].اظہاریہ)); }
#سطر 3278 "src/xkbcomp/parser.c"
    توڑو;

  حالت 157: /* Expr: Lhs "=" Expr  */
#سطر 840 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_تفویض, (yyvsp[-2].اظہاریہ), (yyvsp[0].اظہاریہ)); }
#سطر 3284 "src/xkbcomp/parser.c"
    توڑو;

  حالت 158: /* Expr: Term  */
#سطر 842 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[0].اظہاریہ); }
#سطر 3290 "src/xkbcomp/parser.c"
    توڑو;

  حالت 159: /* Term: "-" Term  */
#سطر 846 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Unary(STMT_اظہاریہ_NEGATE, (yyvsp[0].اظہاریہ)); }
#سطر 3296 "src/xkbcomp/parser.c"
    توڑو;

  حالت 160: /* Term: "+" Term  */
#سطر 848 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Unary(STMT_اظہاریہ_UNARY_PLUS, (yyvsp[0].اظہاریہ)); }
#سطر 3302 "src/xkbcomp/parser.c"
    توڑو;

  حالت 161: /* Term: "!" Term  */
#سطر 850 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Unary(STMT_اظہاریہ_NOT, (yyvsp[0].اظہاریہ)); }
#سطر 3308 "src/xkbcomp/parser.c"
    توڑو;

  حالت 162: /* Term: "~" Term  */
#سطر 852 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Unary(STMT_اظہاریہ_INVERT, (yyvsp[0].اظہاریہ)); }
#سطر 3314 "src/xkbcomp/parser.c"
    توڑو;

  حالت 163: /* Term: Lhs  */
#سطر 854 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[0].اظہاریہ); }
#سطر 3320 "src/xkbcomp/parser.c"
    توڑو;

  حالت 164: /* Term: FieldSpec "(" ExprList ")"  */
#سطر 856 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Action((yyvsp[-3].atom), (yyvsp[-1].اظہاریہ_List).سر); }
#سطر 3326 "src/xkbcomp/parser.c"
    توڑو;

  حالت 165: /* Term: Actions  */
#سطر 858 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[0].اظہاریہ); }
#سطر 3332 "src/xkbcomp/parser.c"
    توڑو;

  حالت 166: /* Term: Terminal  */
#سطر 860 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[0].اظہاریہ); }
#سطر 3338 "src/xkbcomp/parser.c"
    توڑو;

  حالت 167: /* Term: "(" Expr ")"  */
#سطر 862 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[-1].اظہاریہ); }
#سطر 3344 "src/xkbcomp/parser.c"
    توڑو;

  حالت 168: /* MultiActionList: MultiActionList "," Action  */
#سطر 866 "src/xkbcomp/parser.y"
                        {
                            اظہاریہ_Def *اظہاریہ = اظہاریہ_Create_Action_List((yyvsp[0].اظہاریہ));
                            (yyval.اظہاریہ_List) = (yyvsp[-2].اظہاریہ_List);
                            (yyval.اظہاریہ_List).آخری->common.اگلا = &اظہاریہ->common; (yyval.اظہاریہ_List).آخری = اظہاریہ;
                        }
#سطر 3354 "src/xkbcomp/parser.c"
    توڑو;

  حالت 169: /* MultiActionList: MultiActionList "," Actions  */
#سطر 872 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List) = (yyvsp[-2].اظہاریہ_List); (yyval.اظہاریہ_List).آخری->common.اگلا = &(yyvsp[0].اظہاریہ)->common; (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 3360 "src/xkbcomp/parser.c"
    توڑو;

  حالت 170: /* MultiActionList: Action  */
#سطر 874 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = اظہاریہ_Create_Action_List((yyvsp[0].اظہاریہ)); }
#سطر 3366 "src/xkbcomp/parser.c"
    توڑو;

  حالت 171: /* MultiActionList: NonEmptyActions  */
#سطر 876 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 3372 "src/xkbcomp/parser.c"
    توڑو;

  حالت 172: /* ActionList: ActionList "," Action  */
#سطر 880 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List) = (yyvsp[-2].اظہاریہ_List); (yyval.اظہاریہ_List).آخری->common.اگلا = &(yyvsp[0].اظہاریہ)->common; (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 3378 "src/xkbcomp/parser.c"
    توڑو;

  حالت 173: /* ActionList: Action  */
#سطر 882 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 3384 "src/xkbcomp/parser.c"
    توڑو;

  حالت 174: /* NonEmptyActions: "{" ActionList "}"  */
#سطر 886 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Action_List((yyvsp[-1].اظہاریہ_List).سر); }
#سطر 3390 "src/xkbcomp/parser.c"
    توڑو;

  حالت 175: /* Actions: NonEmptyActions  */
#سطر 890 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[0].اظہاریہ); }
#سطر 3396 "src/xkbcomp/parser.c"
    توڑو;

  حالت 176: /* Actions: "{" "}"  */
#سطر 892 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Action_List(تہی); }
#سطر 3402 "src/xkbcomp/parser.c"
    توڑو;

  حالت 177: /* Action: FieldSpec "(" ExprList ")"  */
#سطر 896 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Action((yyvsp[-3].atom), (yyvsp[-1].اظہاریہ_List).سر); }
#سطر 3408 "src/xkbcomp/parser.c"
    توڑو;

  حالت 178: /* Lhs: FieldSpec  */
#سطر 900 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Ident((yyvsp[0].atom)); }
#سطر 3414 "src/xkbcomp/parser.c"
    توڑو;

  حالت 179: /* Lhs: FieldSpec "." FieldSpec  */
#سطر 902 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_میدان_Ref((yyvsp[-2].atom), (yyvsp[0].atom)); }
#سطر 3420 "src/xkbcomp/parser.c"
    توڑو;

  حالت 180: /* Lhs: FieldSpec "[" Expr "]"  */
#سطر 904 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Array_Ref(XKB_ATOM_NONE, (yyvsp[-3].atom), (yyvsp[-1].اظہاریہ)); }
#سطر 3426 "src/xkbcomp/parser.c"
    توڑو;

  حالت 181: /* Lhs: FieldSpec "." FieldSpec "[" Expr "]"  */
#سطر 906 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Array_Ref((yyvsp[-5].atom), (yyvsp[-3].atom), (yyvsp[-1].اظہاریہ)); }
#سطر 3432 "src/xkbcomp/parser.c"
    توڑو;

  حالت 182: /* OptTerminal: Terminal  */
#سطر 910 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[0].اظہاریہ); }
#سطر 3438 "src/xkbcomp/parser.c"
    توڑو;

  حالت 183: /* OptTerminal: %empty  */
#سطر 911 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = تہی; }
#سطر 3444 "src/xkbcomp/parser.c"
    توڑو;

  حالت 184: /* Terminal: String  */
#سطر 915 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_String((yyvsp[0].atom)); }
#سطر 3450 "src/xkbcomp/parser.c"
    توڑو;

  حالت 185: /* Terminal: Integer  */
#سطر 917 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Integer((yyvsp[0].عدد)); }
#سطر 3456 "src/xkbcomp/parser.c"
    توڑو;

  حالت 186: /* Terminal: Float  */
#سطر 919 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_Float(/* Discard $1 */); }
#سطر 3462 "src/xkbcomp/parser.c"
    توڑو;

  حالت 187: /* Terminal: "key name"  */
#سطر 921 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_کلید_Name((yyvsp[0].atom)); }
#سطر 3468 "src/xkbcomp/parser.c"
    توڑو;

  حالت 188: /* MultiKeySymList: MultiKeySymList "," KeySymLit  */
#سطر 925 "src/xkbcomp/parser.y"
                        {
                            اظہاریہ_Def *اظہاریہ = اظہاریہ_Create_کلید_Sym_List((yyvsp[0].keysym));
                            (yyval.اظہاریہ_List) = (yyvsp[-2].اظہاریہ_List);
                            (yyval.اظہاریہ_List).آخری->common.اگلا = &اظہاریہ->common; (yyval.اظہاریہ_List).آخری = اظہاریہ;
                        }
#سطر 3478 "src/xkbcomp/parser.c"
    توڑو;

  حالت 189: /* MultiKeySymList: MultiKeySymList "," KeySyms  */
#سطر 931 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List) = (yyvsp[-2].اظہاریہ_List); (yyval.اظہاریہ_List).آخری->common.اگلا = &(yyvsp[0].اظہاریہ)->common; (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 3484 "src/xkbcomp/parser.c"
    توڑو;

  حالت 190: /* MultiKeySymList: KeySymLit  */
#سطر 933 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = اظہاریہ_Create_کلید_Sym_List((yyvsp[0].keysym)); }
#سطر 3490 "src/xkbcomp/parser.c"
    توڑو;

  حالت 191: /* MultiKeySymList: NonEmptyKeySyms  */
#سطر 935 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ_List).سر = (yyval.اظہاریہ_List).آخری = (yyvsp[0].اظہاریہ); }
#سطر 3496 "src/xkbcomp/parser.c"
    توڑو;

  حالت 192: /* KeySymList: KeySymList "," KeySymLit  */
#سطر 939 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Append_کلید_Sym_List((yyvsp[-2].اظہاریہ), (yyvsp[0].keysym)); }
#سطر 3502 "src/xkbcomp/parser.c"
    توڑو;

  حالت 193: /* KeySymList: KeySymList "," "string literal"  */
#سطر 941 "src/xkbcomp/parser.y"
                        {
                            (yyval.اظہاریہ) = اظہاریہ_کلید_Sym_List_Append_String(param->scanner, (yyvsp[-2].اظہاریہ), (yyvsp[0].str));
                            آزاد((yyvsp[0].str));
                            اگر (!(yyval.اظہاریہ))
                                YYERROR;
                        }
#سطر 3513 "src/xkbcomp/parser.c"
    توڑو;

  حالت 194: /* KeySymList: KeySymLit  */
#سطر 948 "src/xkbcomp/parser.y"
                        {
                            (yyval.اظہاریہ) = اظہاریہ_Create_کلید_Sym_List((yyvsp[0].keysym));
                            اگر (!(yyval.اظہاریہ))
                                YYERROR;
                        }
#سطر 3523 "src/xkbcomp/parser.c"
    توڑو;

  حالت 195: /* KeySymList: "string literal"  */
#سطر 954 "src/xkbcomp/parser.y"
                        {
                            (yyval.اظہاریہ) = اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت);
                            اگر (!(yyval.اظہاریہ))
                                YYERROR;
                            (yyval.اظہاریہ) = اظہاریہ_کلید_Sym_List_Append_String(param->scanner, (yyval.اظہاریہ), (yyvsp[0].str));
                            آزاد((yyvsp[0].str));
                            اگر (!(yyval.اظہاریہ))
                                YYERROR;
                        }
#سطر 3537 "src/xkbcomp/parser.c"
    توڑو;

  حالت 196: /* NonEmptyKeySyms: "{" KeySymList "}"  */
#سطر 966 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[-1].اظہاریہ); }
#سطر 3543 "src/xkbcomp/parser.c"
    توڑو;

  حالت 197: /* NonEmptyKeySyms: "string literal"  */
#سطر 968 "src/xkbcomp/parser.y"
                        {
                            (yyval.اظہاریہ) = اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت);
                            اگر (!(yyval.اظہاریہ))
                                YYERROR;
                            (yyval.اظہاریہ) = اظہاریہ_کلید_Sym_List_Append_String(param->scanner, (yyval.اظہاریہ), (yyvsp[0].str));
                            آزاد((yyvsp[0].str));
                            اگر (!(yyval.اظہاریہ))
                                YYERROR;
                        }
#سطر 3557 "src/xkbcomp/parser.c"
    توڑو;

  حالت 198: /* KeySyms: NonEmptyKeySyms  */
#سطر 980 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = (yyvsp[0].اظہاریہ); }
#سطر 3563 "src/xkbcomp/parser.c"
    توڑو;

  حالت 199: /* KeySyms: "{" "}"  */
#سطر 982 "src/xkbcomp/parser.y"
                        { (yyval.اظہاریہ) = اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت); }
#سطر 3569 "src/xkbcomp/parser.c"
    توڑو;

  حالت 200: /* KeySym: KeySymLit  */
#سطر 986 "src/xkbcomp/parser.y"
                        { (yyval.keysym) = (yyvsp[0].keysym); }
#سطر 3575 "src/xkbcomp/parser.c"
    توڑو;

  حالت 201: /* KeySym: "string literal"  */
#سطر 988 "src/xkbcomp/parser.y"
                        {
                            (yyval.keysym) = KeysymParseString(param->scanner, (yyvsp[0].str));
                            آزاد((yyvsp[0].str));
                            اگر ((yyval.keysym) == XKB_کلید_No_علامت)
                                YYERROR;
                        }
#سطر 3586 "src/xkbcomp/parser.c"
    توڑو;

  حالت 202: /* KeySymLit: "identifier"  */
#سطر 997 "src/xkbcomp/parser.y"
                        {
                            اگر (!resolve_keysym(param, (yyvsp[0].sval), &(yyval.keysym))) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (غیرعلامتی صحیح) (yyvsp[0].sval).len, (yyvsp[0].sval).آغاز
                                );
                                (yyval.keysym) = XKB_کلید_No_علامت;
                            }
                        }
#سطر 3602 "src/xkbcomp/parser.c"
    توڑو;

  حالت 203: /* KeySymLit: "section"  */
#سطر 1009 "src/xkbcomp/parser.y"
                                { (yyval.keysym) = XKB_کلید_section; }
#سطر 3608 "src/xkbcomp/parser.c"
    توڑو;

  حالت 204: /* KeySymLit: "decimal digit"  */
#سطر 1011 "src/xkbcomp/parser.y"
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            (yyval.keysym) = XKB_کلید_0 + (xkb_keysym_t) (yyvsp[0].عدد);
                        }
#سطر 3621 "src/xkbcomp/parser.c"
    توڑو;

  حالت 205: /* KeySymLit: "integer literal"  */
#سطر 1020 "src/xkbcomp/parser.y"
                        {
                            اگر ((yyvsp[0].عدد) < XKB_KEYSYM_MIN) {
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
                                (yyval.keysym) = XKB_کلید_No_علامت;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            ورنہ {
                                /* Any other numeric value */
                                اگر ((yyvsp[0].عدد) <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    (yyval.keysym) = (xkb_keysym_t) (yyvsp[0].عدد);
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        (yyval.keysym), تہی, (yyval.keysym), "%#06"PRIx32, "");
                                } ورنہ {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", (yyvsp[0].عدد), (yyvsp[0].عدد)
                                    );
                                    (yyval.keysym) = XKB_کلید_No_علامت;
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
#سطر 3683 "src/xkbcomp/parser.c"
    توڑو;

  حالت 206: /* SignedNumber: "-" Number  */
#سطر 1079 "src/xkbcomp/parser.y"
                                        { (yyval.عدد) = -(yyvsp[0].عدد); }
#سطر 3689 "src/xkbcomp/parser.c"
    توڑو;

  حالت 207: /* SignedNumber: Number  */
#سطر 1080 "src/xkbcomp/parser.y"
                                        { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3695 "src/xkbcomp/parser.c"
    توڑو;

  حالت 208: /* Number: "float literal"  */
#سطر 1083 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3701 "src/xkbcomp/parser.c"
    توڑو;

  حالت 209: /* Number: "decimal digit"  */
#سطر 1084 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3707 "src/xkbcomp/parser.c"
    توڑو;

  حالت 210: /* Number: "integer literal"  */
#سطر 1085 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3713 "src/xkbcomp/parser.c"
    توڑو;

  حالت 211: /* Float: "float literal"  */
#سطر 1088 "src/xkbcomp/parser.y"
                                { (yyval.عدد) = 0; }
#سطر 3719 "src/xkbcomp/parser.c"
    توڑو;

  حالت 212: /* Integer: "integer literal"  */
#سطر 1091 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3725 "src/xkbcomp/parser.c"
    توڑو;

  حالت 213: /* Integer: "decimal digit"  */
#سطر 1092 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3731 "src/xkbcomp/parser.c"
    توڑو;

  حالت 214: /* KeyCode: "integer literal"  */
#سطر 1095 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3737 "src/xkbcomp/parser.c"
    توڑو;

  حالت 215: /* KeyCode: "decimal digit"  */
#سطر 1096 "src/xkbcomp/parser.y"
                                      { (yyval.عدد) = (yyvsp[0].عدد); }
#سطر 3743 "src/xkbcomp/parser.c"
    توڑو;

  حالت 216: /* Ident: "identifier"  */
#سطر 1099 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].sval).آغاز, (yyvsp[0].sval).len); }
#سطر 3749 "src/xkbcomp/parser.c"
    توڑو;

  حالت 217: /* Ident: "default"  */
#سطر 1100 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "default"); }
#سطر 3755 "src/xkbcomp/parser.c"
    توڑو;

  حالت 218: /* String: "string literal"  */
#سطر 1103 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].str), لڑی_لمبائی((yyvsp[0].str))); آزاد((yyvsp[0].str)); }
#سطر 3761 "src/xkbcomp/parser.c"
    توڑو;

  حالت 219: /* OptMapName: MapName  */
#سطر 1106 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#سطر 3767 "src/xkbcomp/parser.c"
    توڑو;

  حالت 220: /* OptMapName: %empty  */
#سطر 1107 "src/xkbcomp/parser.y"
                                { (yyval.str) = تہی; }
#سطر 3773 "src/xkbcomp/parser.c"
    توڑو;

  حالت 221: /* MapName: "string literal"  */
#سطر 1110 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#سطر 3779 "src/xkbcomp/parser.c"
    توڑو;


#سطر 3783 "src/xkbcomp/parser.c"

      طےشدہ: توڑو;
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
  YY_علامت_PRINT ("-> $$ =", YY_CAST (yysymbol_قسم_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ثابت صحیح yylhs = yyr1[yyn] - YYNTOKENS;
    ثابت صحیح yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  جاؤ yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  اگر (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        حرف ثابت *yymsgp = YY_("syntax error");
        صحیح yysyntax_error_حالت;
        yysyntax_error_حالت = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        اگر (yysyntax_error_حالت == 0)
          yymsgp = yymsg;
        ورنہ اگر (yysyntax_error_حالت == -1)
          {
            اگر (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (حرف *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            اگر (yymsg)
              {
                yysyntax_error_حالت
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            ورنہ
              {
                yymsg = yymsgbuf;
                yymsg_alloc = حجم yymsgbuf;
                yysyntax_error_حالت = YYENOMEM;
              }
          }
        yyerror (param, yymsgp);
        اگر (yysyntax_error_حالت == YYENOMEM)
          YYNOMEM;
      }
    }

  اگر (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      اگر (yychar <= انتہا_کا_FILE)
        {
          /* Return failure if at end of input.  */
          اگر (yychar == انتہا_کا_FILE)
            YYABORT;
        }
      ورنہ
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, param);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  جاؤ yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  اگر (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ڈھیر_PRINT (yyss, yyssp);
  yystate = *yyssp;
  جاؤ yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  برائے (;;)
    {
      yyn = yypact[yystate];
      اگر (!yypact_قدر_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          اگر (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              اگر (0 < yyn)
                توڑو;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      اگر (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_علامت (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_ڈھیر_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_انتہا


  /* Shift the error token.  */
  YY_علامت_PRINT ("Shifting", YY_ACCESSING_علامت (yyn), yyvsp, yylsp);

  yystate = yyn;
  جاؤ yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  جاؤ yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  جاؤ yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  جاؤ yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  اگر (yychar != YYEMPTY)
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
  YY_ڈھیر_PRINT (yyss, yyssp);
  جب (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_علامت (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#اگر_غیر_تعریف_شدہ yyoverflow
  اگر (yyss != yyssa)
    YYSTACK_FREE (yyss);
#اگر_ختم
  اگر (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  واپس yyresult;
}

#سطر 1113 "src/xkbcomp/parser.y"


/* Parse a specific section */
XkbFile *
parse(ساخت xkb_context *ctx, ثابت ساخت parser_keymap_config *config,
      ساخت scanner *scanner, ثابت حرف *map)
{
    صحیح ret;
    XkbFile *پہلا = تہی;
    ساخت parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = تہی,
        .config = *config,
        .مزید_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    جب ((ret = yyparse(&param)) == 0 && param.مزید_maps) {
        اگر (map) {
            اگر (streq_not_null(map, param.rtrn->name))
                واپس param.rtrn;
            ورنہ
                FreeXkbFile(param.rtrn);
        }
        ورنہ {
            اگر (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(پہلا);
                واپس param.rtrn;
            }
            ورنہ اگر (!پہلا) {
                پہلا = param.rtrn;
            }
            ورنہ {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = تہی;
    }

    اگر (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(پہلا);
        FreeXkbFile(param.rtrn);
        واپس تہی;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    اگر (پہلا && پہلا->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, محفوظ_map_name(پہلا->name));

    واپس پہلا;
}

/* Parse the next section */
bool
parse_اگلا(ساخت xkb_context *ctx,
           ثابت ساخت parser_keymap_config *config,
           ساخت scanner *scanner, XkbFile **xkb_file)
{
    صحیح ret;
    ساخت parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = تہی,
        .config = *config,
        .مزید_maps = false,
    };

    اگر ((ret = yyparse(&param)) == 0 && param.مزید_maps) {
        *xkb_file = param.rtrn;
        واپس true;
    } ورنہ {
        FreeXkbFile(param.rtrn);
        *xkb_file = تہی;
        واپس (ret == 0);
    }
}
