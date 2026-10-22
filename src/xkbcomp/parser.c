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
#परिभाषय YYBISON 30802

/* Bison version string.  */
#परिभाषय YYBISON_संस्करणम् "3.8.2"

/* Skeleton name.  */
#परिभाषय YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#परिभाषय YYPURE 1

/* Push parsers.  */
#परिभाषय YYPUSH 0

/* Pull parsers.  */
#परिभाषय YYPULL 1


/* Substitute the variable and function names.  */
#परिभाषय yyparse         _xkbcommon_parse
#परिभाषय yylex           _xkbcommon_lex
#परिभाषय yyerror         _xkbcommon_error
#परिभाषय yydebug         _xkbcommon_debug
#परिभाषय yynerrs         _xkbcommon_nerrs

/* First part of user prologue.  */
#पङ्क्तिः 22 "src/xkbcomp/parser.y"

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "keysym.h"
#अन्तर्भाव "xkbcomp/xkbcomp-priv.h"
#अन्तर्भाव "xkbcomp/parser-priv.h"
#अन्तर्भाव "xkbcomp/ast-build.h"

संरचना parser_param {
    संरचना xkb_context *ctx;
    संरचना scanner *scanner;
    XkbFile *rtrn;
    संरचना parser_keymap_config config;
    bool अधिकम्_maps;
};

#परिभाषय parser_log_with_संहिता(param, level, verbosity, log_सन्देशः_id, fmt, ...)   \
    scanner_log_with_संहिता((param)->scanner, level, verbosity, log_सन्देशः_id, fmt,\
                          ##__VA_ARGS__)

#परिभाषय parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#परिभाषय parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#परिभाषय parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

स्थिर शून्य
_xkbcommon_error(संरचना parser_param *param, ध्रुव वर्ण *सन्देशः)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", सन्देशः);
}

स्थिर bool
resolve_keysym(संरचना parser_param *param, संरचना sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    यदि (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_कुञ्चिका_No_चिह्नम्;
        प्रत्यागम true;
    }

    यदि (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_कुञ्चिका_Void_चिह्नम्;
        प्रत्यागम true;
    }

    /* xkb_keysym_from_name needs a C string. */
    वर्ण buf[XKB_KEYSYM_NAME_MAX_आकार];
    यदि (name.len >= परिमाण(buf)) {
        प्रत्यागम false;
    }
    स्मृतिं_प्रतिलिख(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    यदि (sym != XKB_कुञ्चिका_No_चिह्नम्) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        प्रत्यागम true;
    }

    प्रत्यागम false;
}

#परिभाषय param_scanner param->scanner

#पङ्क्तिः 150 "src/xkbcomp/parser.c"

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
#  यदि defined __cplusplus
#   यदि 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   अन्यथा
#    define YY_NULLPTR 0
#   endif
#  अन्यथा
#   define YY_NULLPTR ((शून्य*)0)
#  endif
# endif

#अन्तर्भाव "parser.h"
/* Symbol kind.  */
गणना yysymbol_विधा_t
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
  YYSYMBOL_कुञ्चिका = 21,                       /* "key"  */
  YYSYMBOL_ALIAS = 22,                     /* "alias"  */
  YYSYMBOL_गणः = 23,                     /* "group"  */
  YYSYMBOL_MODIFIER_MAP = 24,              /* "modifier_map"  */
  YYSYMBOL_INDICATOR = 25,                 /* "indicator"  */
  YYSYMBOL_SHAPE = 26,                     /* "shape"  */
  YYSYMBOL_कुञ्चिकाः = 27,                      /* "keys"  */
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
  YYSYMBOL_ALPHANUMERIC_कुञ्चिकाः = 61,         /* "alphanumeric_keys"  */
  YYSYMBOL_MODIFIER_कुञ्चिकाः = 62,             /* "modifier_keys"  */
  YYSYMBOL_KEYPAD_कुञ्चिकाः = 63,               /* "keypad_keys"  */
  YYSYMBOL_FUNCTION_कुञ्चिकाः = 64,             /* "function_keys"  */
  YYSYMBOL_ALTERNATE_गणः = 65,           /* "alternate_group"  */
  YYSYMBOL_DEPRECATED = 66,                /* "deprecated"  */
  YYSYMBOL_YYACCEPT = 67,                  /* $accept  */
  YYSYMBOL_XkbFile = 68,                   /* XkbFile  */
  YYSYMBOL_Xkb_संयुक्त_Map = 69,           /* XkbCompositeMap  */
  YYSYMBOL_Xkb_संयुक्त_Type = 70,          /* XkbCompositeType  */
  YYSYMBOL_XkbMapConfigList = 71,          /* XkbMapConfigList  */
  YYSYMBOL_XkbMapConfig = 72,              /* XkbMapConfig  */
  YYSYMBOL_FileType = 73,                  /* FileType  */
  YYSYMBOL_OptFlags = 74,                  /* OptFlags  */
  YYSYMBOL_Flags = 75,                     /* Flags  */
  YYSYMBOL_Flag = 76,                      /* Flag  */
  YYSYMBOL_DeclList = 77,                  /* DeclList  */
  YYSYMBOL_Decl = 78,                      /* Decl  */
  YYSYMBOL_VarDecl = 79,                   /* VarDecl  */
  YYSYMBOL_कुञ्चिका_Name_Decl = 80,               /* KeyNameDecl  */
  YYSYMBOL_कुञ्चिका_Alias_Decl = 81,              /* KeyAliasDecl  */
  YYSYMBOL_VModDecl = 82,                  /* VModDecl  */
  YYSYMBOL_VModDefList = 83,               /* VModDefList  */
  YYSYMBOL_VModDef = 84,                   /* VModDef  */
  YYSYMBOL_InterpretDecl = 85,             /* InterpretDecl  */
  YYSYMBOL_Interpret_मेलनम् = 86,            /* InterpretMatch  */
  YYSYMBOL_VarDeclList = 87,               /* VarDeclList  */
  YYSYMBOL_कुञ्चिका_Type_Decl = 88,               /* KeyTypeDecl  */
  YYSYMBOL_SymbolsDecl = 89,               /* SymbolsDecl  */
  YYSYMBOL_OptSymbolsBody = 90,            /* OptSymbolsBody  */
  YYSYMBOL_SymbolsBody = 91,               /* SymbolsBody  */
  YYSYMBOL_SymbolsVarDecl = 92,            /* SymbolsVarDecl  */
  YYSYMBOL_Multi_कुञ्चिका_Sym_Or_Action_List = 93,   /* MultiKeySymOrActionList  */
  YYSYMBOL_No_चिह्नम्_Or_Action_List = 94,      /* NoSymbolOrActionList  */
  YYSYMBOL_गणः_Compat_Decl = 95,           /* GroupCompatDecl  */
  YYSYMBOL_ModMapDecl = 96,                /* ModMapDecl  */
  YYSYMBOL_कुञ्चिका_Or_कुञ्चिका_Sym_List = 97,           /* KeyOrKeySymList  */
  YYSYMBOL_कुञ्चिका_Or_कुञ्चिका_Sym = 98,               /* KeyOrKeySym  */
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
  YYSYMBOL_कुञ्चिकाः_2 = 109,                     /*कुञ्चिकाः*/
  YYSYMBOL_कुञ्चिका_2 = 110,                      /*कुञ्चिका*/
  YYSYMBOL_OverlayDecl = 111,              /* OverlayDecl  */
  YYSYMBOL_Overlay_कुञ्चिका_List = 112,           /* OverlayKeyList  */
  YYSYMBOL_Overlay_कुञ्चिका = 113,               /* OverlayKey  */
  YYSYMBOL_OutlineList = 114,              /* OutlineList  */
  YYSYMBOL_Outline_अन्तर्_List = 115,            /* OutlineInList  */
  YYSYMBOL_CoordList = 116,                /* CoordList  */
  YYSYMBOL_Coord = 117,                    /* Coord  */
  YYSYMBOL_DoodadDecl = 118,               /* DoodadDecl  */
  YYSYMBOL_DoodadType = 119,               /* DoodadType  */
  YYSYMBOL_क्षेत्रम्_Spec = 120,                /* FieldSpec  */
  YYSYMBOL_Element = 121,                  /* Element  */
  YYSYMBOL_Opt_Merge_बहुलकम् = 122,             /* OptMergeMode  */
  YYSYMBOL_Merge_बहुलकम् = 123,                /* MergeMode  */
  YYSYMBOL_व्यञ्जकम्_List = 124,                 /* ExprList  */
  YYSYMBOL_व्यञ्जकम् = 125,                     /*व्यञ्जकम्*/
  YYSYMBOL_पदांशः = 126,                     /*पदांशः*/
  YYSYMBOL_MultiActionList = 127,          /* MultiActionList  */
  YYSYMBOL_ActionList = 128,               /* ActionList  */
  YYSYMBOL_Non_रिक्तानि_Actions = 129,          /* NonEmptyActions  */
  YYSYMBOL_Actions = 130,                  /* Actions  */
  YYSYMBOL_Action = 131,                   /* Action  */
  YYSYMBOL_Lhs = 132,                      /* Lhs  */
  YYSYMBOL_OptTerminal = 133,              /* OptTerminal  */
  YYSYMBOL_Terminal = 134,                 /* Terminal  */
  YYSYMBOL_Multi_कुञ्चिका_Sym_List = 135,          /* MultiKeySymList  */
  YYSYMBOL_कुञ्चिका_Sym_List = 136,               /* KeySymList  */
  YYSYMBOL_Non_रिक्तानि_कुञ्चिका_Syms = 137,          /* NonEmptyKeySyms  */
  YYSYMBOL_कुञ्चिका_Syms = 138,                  /* KeySyms  */
  YYSYMBOL_कुञ्चिका_Sym = 139,                   /* KeySym  */
  YYSYMBOL_कुञ्चिका_Sym_Lit = 140,                /* KeySymLit  */
  YYSYMBOL_Signed_सङ्ख्या = 141,             /* SignedNumber  */
  YYSYMBOL_सङ्ख्या = 142,                   /*सङ्ख्या*/
  YYSYMBOL_Float = 143,                    /* Float  */
  YYSYMBOL_Integer = 144,                  /* Integer  */
  YYSYMBOL_कुञ्चिका_संहिता = 145,                  /* KeyCode  */
  YYSYMBOL_Ident = 146,                    /* Ident  */
  YYSYMBOL_String = 147,                   /* String  */
  YYSYMBOL_OptMapName = 148,               /* OptMapName  */
  YYSYMBOL_MapName = 149                   /* MapName  */
};
प्रकारनिर्देश गणना yysymbol_विधा_t yysymbol_विधा_t;




#यदि_परिभाषितम् ह्रस्व
# undef ह्रस्व
#यदि_अन्तः

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#यदि_अपरिभाषितम् __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# यदि defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#यदि_अन्तः

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#यदि_परिभाषितम् __INT_LEAST8_MAX__
प्रकारनिर्देश __INT_LEAST8_TYPE__ yytype_int8;
#नोचेद्यदि defined YY_STDINT_H
प्रकारनिर्देश int_least8_t yytype_int8;
#अन्यथा
प्रकारनिर्देश चिह्नित वर्ण yytype_int8;
#यदि_अन्तः

#यदि_परिभाषितम् __INT_LEAST16_MAX__
प्रकारनिर्देश __INT_LEAST16_TYPE__ yytype_int16;
#नोचेद्यदि defined YY_STDINT_H
प्रकारनिर्देश int_least16_t yytype_int16;
#अन्यथा
प्रकारनिर्देश ह्रस्व yytype_int16;
#यदि_अन्तः

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#यदि_परिभाषितम् __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#यदि_अन्तः

#यदि defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
प्रकारनिर्देश __UINT_LEAST8_TYPE__ yytype_uint8;
#नोचेद्यदि (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= अधिकतम_पूर्णाङ्कः)
प्रकारनिर्देश uint_least8_t yytype_uint8;
#नोचेद्यदि !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= अधिकतम_पूर्णाङ्कः
प्रकारनिर्देश अचिह्नित वर्ण yytype_uint8;
#अन्यथा
प्रकारनिर्देश ह्रस्व yytype_uint8;
#यदि_अन्तः

#यदि defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
प्रकारनिर्देश __UINT_LEAST16_TYPE__ yytype_uint16;
#नोचेद्यदि (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= अधिकतम_पूर्णाङ्कः)
प्रकारनिर्देश uint_least16_t yytype_uint16;
#नोचेद्यदि !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= अधिकतम_पूर्णाङ्कः
प्रकारनिर्देश अचिह्नित ह्रस्व yytype_uint16;
#अन्यथा
प्रकारनिर्देश पूर्णाङ्क yytype_uint16;
#यदि_अन्तः

#यदि_अपरिभाषितम् YYPTRDIFF_T
# यदि defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_अधिकतमम् __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_अधिकतमम् PTRDIFF_MAX
# अन्यथा
#  define YYPTRDIFF_T दीर्घ
#  define YYPTRDIFF_अधिकतमम् LONG_MAX
# endif
#यदि_अन्तः

#यदि_अपरिभाषितम् YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined आकार_t
#  define YYSIZE_T आकार_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T आकार_t
# अन्यथा
#  define YYSIZE_T अचिह्नित
# endif
#यदि_अन्तः

#परिभाषय YYSIZE_अधिकतमम्                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_अधिकतमम् < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_अधिकतमम्                         \
            : YY_CAST (YYSIZE_T, -1)))

#परिभाषय YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, परिमाण (X))


/* Stored state numbers (used for stacks). */
प्रकारनिर्देश yytype_int16 yy_state_t;

/* State numbers in computations.  */
प्रकारनिर्देश पूर्णाङ्क yy_state_fast_t;

#यदि_अपरिभाषितम् YY_
# यदि defined YYENABLE_NLS && YYENABLE_NLS
#  यदि ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#यदि_अन्तः


#यदि_अपरिभाषितम् YY_ATTRIBUTE_विशुद्धम्
# यदि defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_विशुद्धम् __attribute__ ((__pure__))
# अन्यथा
#  define YY_ATTRIBUTE_विशुद्धम्
# endif
#यदि_अन्तः

#यदि_अपरिभाषितम् YY_ATTRIBUTE_अप्रयुक्तम्
# यदि defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_अप्रयुक्तम् __attribute__ ((__unused__))
# अन्यथा
#  define YY_ATTRIBUTE_अप्रयुक्तम्
# endif
#यदि_अन्तः

/* Suppress unused-variable warnings by "using" E.  */
#यदि ! defined lint || defined __GNUC__
# define YY_USE(E) ((शून्य) (E))
#अन्यथा
# define YY_USE(E) /*रिक्तानि*/
#यदि_अन्तः

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#यदि defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# यदि __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# अन्यथा
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः      \
    _Pragma ("GCC diagnostic pop")
#अन्यथा
# define YY_INITIAL_मानम्(मानम्_2) मानम्_2
#यदि_अन्तः
#यदि_अपरिभाषितम् YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः
#यदि_अन्तः
#यदि_अपरिभाषितम् YY_INITIAL_मानम्
# define YY_INITIAL_मानम्(मानम्_2) /* Nothing. */
#यदि_अन्तः

#यदि defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_अन्तः            \
    _Pragma ("GCC diagnostic pop")
#यदि_अन्तः
#यदि_अपरिभाषितम् YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_अन्तः
#यदि_अन्तः


#परिभाषय YY_ASSERT(E) ((शून्य) (0 && (E)))

#यदि 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  यदि YYSTACK_USE_ALLOCA
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
#    यदि ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
#  define YYSTACK_FREE(Ptr) कुरु { /*रिक्तानि*/; } यावत् (0)
#  ifndef YYSTACK_ALLOC_अधिकतमम्
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_अधिकतमम् 4032 /* reasonable circa 2006 */
#  endif
# अन्यथा
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_अधिकतमम्
#   define YYSTACK_ALLOC_अधिकतमम् YYSIZE_अधिकतमम्
#  endif
#  यदि (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined स्मृतिं_देहि) \
             && (defined YYFREE || defined मुक्त)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC स्मृतिं_देहि
#   यदि ! defined स्मृतिं_देहि && ! defined EXIT_SUCCESS
शून्य *स्मृतिं_देहि (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE मुक्त
#   यदि ! defined मुक्त && ! defined EXIT_SUCCESS
शून्य मुक्त (शून्य *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#यदि_अन्तः /* 1 */

#यदि (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
सङ्घ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_अधिकतमम् (YYSIZEOF (सङ्घ yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_अष्टकानि(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_अधिकतमम्)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(राशिः_alloc, राशिः_2)                           \
    कुरु                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->राशिः_alloc, राशिः_2, yysize);                    \
        राशिः_2 = &yyptr->राशिः_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*राशिः_2) + YYSTACK_GAP_अधिकतमम्; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    यावत् (0)

#यदि_अन्तः

#यदि defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  यदि defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, गणनम्_2) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (गणनम्_2)) * परिमाण (*(Src)))
#  अन्यथा
#   define YYCOPY(Dst, Src, गणनम्_2)              \
      कुरु                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          प्रति (yyi = 0; yyi < (गणनम्_2); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      यावत् (0)
#  endif
# endif
#यदि_अन्तः /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#परिभाषय YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#परिभाषय YYLAST   945

/* YYNTOKENS -- Number of terminals.  */
#परिभाषय YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#परिभाषय YYNNTS  83
/* YYNRULES -- Number of rules.  */
#परिभाषय YYNRULES  221
/* YYNSTATES -- Number of states.  */
#परिभाषय YYNSTATES  386

/* YYMAXUTOK -- Last valid token kind.  */
#परिभाषय YYMAXUTOK   257


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#परिभाषय YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_विधा_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
स्थिर ध्रुव yytype_int8 yytranslate[] =
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

#यदि YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
स्थिर ध्रुव yytype_int16 yyrline[] =
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
#यदि_अन्तः

/** Accessing symbol of state STATE.  */
#परिभाषय YY_ACCESSING_चिह्नम्(State) YY_CAST (yysymbol_विधा_t, yystos[State])

#यदि 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
स्थिर ध्रुव वर्ण *yysymbol_name (yysymbol_विधा_t yysymbol) YY_ATTRIBUTE_अप्रयुक्तम्;

स्थिर ध्रुव वर्ण *
yysymbol_name (yysymbol_विधा_t yysymbol)
{
  स्थिर ध्रुव वर्ण *ध्रुव yy_sname[] =
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
  प्रत्यागम yy_sname[yysymbol];
}
#यदि_अन्तः

#परिभाषय YYPACT_NINF (-225)

#परिभाषय yypact_मानम्_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#परिभाषय YYTABLE_NINF (-217)

#परिभाषय yytable_मानम्_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
स्थिर ध्रुव yytype_int16 yypact[] =
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
स्थिर ध्रुव yytype_uint8 yydefact[] =
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
स्थिर ध्रुव yytype_int16 yypgoto[] =
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
स्थिर ध्रुव yytype_int16 yydefgoto[] =
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
स्थिर ध्रुव yytype_int16 yytable[] =
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

स्थिर ध्रुव yytype_int16 yycheck[] =
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
स्थिर ध्रुव yytype_uint8 yystos[] =
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
स्थिर ध्रुव yytype_uint8 yyr1[] =
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
स्थिर ध्रुव yytype_int8 yyr2[] =
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


गणना { YYENOMEM = -2 };

#परिभाषय yyerrok         (yyerrstatus = 0)
#परिभाषय yyclearin       (yychar = YYEMPTY)

#परिभाषय YYACCEPT        गच्छ yyacceptlab
#परिभाषय YYABORT         गच्छ yyabortlab
#परिभाषय YYERROR         गच्छ yyerrorlab
#परिभाषय YYNOMEM         गच्छ yyexhaustedlab


#परिभाषय YYRECOVERING()  (!!yyerrstatus)

#परिभाषय YYBACKUP(Token, मानम्_2)                                    \
  कुरु                                                              \
    यदि (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (मानम्_2);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        गच्छ yybackup;                                            \
      }                                                           \
    अन्यथा                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  यावत् (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#परिभाषय YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#यदि YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(तर्काः_2)                        \
कुरु {                                            \
  यदि (yydebug)                                  \
    YYFPRINTF तर्काः_2;                             \
} यावत् (0)




# define YY_चिह्नम्_PRINT(Title, विधा_2, मानम्_2, Location)                    \
कुरु {                                                                      \
  यदि (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_चिह्नम्_print (stderr,                                            \
                  विधा_2, मानम्_2, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} यावत् (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

स्थिर शून्य
yy_चिह्नम्_मानम्_print (FILE *yyo,
                       yysymbol_विधा_t yykind, YYSTYPE ध्रुव * ध्रुव yyvaluep, संरचना parser_param *param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  यदि (!yyvaluep)
    प्रत्यागम;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

स्थिर शून्य
yy_चिह्नम्_print (FILE *yyo,
                 yysymbol_विधा_t yykind, YYSTYPE ध्रुव * ध्रुव yyvaluep, संरचना parser_param *param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_चिह्नम्_मानम्_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

स्थिर शून्य
yy_राशिः_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  प्रति (; yybottom <= yytop; yybottom++)
    {
      पूर्णाङ्क yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_राशिः_PRINT(Bottom, शिखरम्)                            \
कुरु {                                                            \
  यदि (yydebug)                                                  \
    yy_राशिः_print ((Bottom), (शिखरम्));                           \
} यावत् (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

स्थिर शून्य
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 पूर्णाङ्क yyrule, संरचना parser_param *param)
{
  पूर्णाङ्क yylno = yyrline[yyrule];
  पूर्णाङ्क yynrhs = yyr2[yyrule];
  पूर्णाङ्क yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  प्रति (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_चिह्नम्_print (stderr,
                       YY_ACCESSING_चिह्नम् (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(नियमः)          \
कुरु {                                    \
  यदि (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, नियमः, param); \
} यावत् (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
पूर्णाङ्क yydebug;
#अन्यथा /* !YYDEBUG */
# define YYDPRINTF(तर्काः_2) ((शून्य) 0)
# define YY_चिह्नम्_PRINT(Title, विधा_2, मानम्_2, Location)
# define YY_राशिः_PRINT(Bottom, शिखरम्)
# define YY_REDUCE_PRINT(नियमः)
#यदि_अन्तः /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#यदि_अपरिभाषितम् YYINITDEPTH
# define YYINITDEPTH 200
#यदि_अन्तः

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#यदि_अपरिभाषितम् YYMAXDEPTH
# define YYMAXDEPTH 10000
#यदि_अन्तः


/* Context of a parse error.  */
प्रकारनिर्देश संरचना
{
  yy_state_t *yyssp;
  yysymbol_विधा_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
स्थिर पूर्णाङ्क
yypcontext_expected_tokens (ध्रुव yypcontext_t *yyctx,
                            yysymbol_विधा_t yyarg[], पूर्णाङ्क yyargn)
{
  /* Actual size of YYARG. */
  पूर्णाङ्क yycount = 0;
  पूर्णाङ्क yyn = yypact[+*yyctx->yyssp];
  यदि (!yypact_मानम्_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      पूर्णाङ्क yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      पूर्णाङ्क yychecklim = YYLAST - yyn + 1;
      पूर्णाङ्क yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      पूर्णाङ्क yyx;
      प्रति (yyx = yyxbegin; yyx < yyxend; ++yyx)
        यदि (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_मानम्_is_error (yytable[yyx + yyn]))
          {
            यदि (!yyarg)
              ++yycount;
            अन्यथा यदि (yycount == yyargn)
              प्रत्यागम 0;
            अन्यथा
              yyarg[yycount++] = YY_CAST (yysymbol_विधा_t, yyx);
          }
    }
  यदि (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  प्रत्यागम yycount;
}




#यदि_अपरिभाषितम् yystrlen
# यदि defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, वाक्यदैर्घ्यम् (S)))
# अन्यथा
/* Return the length of YYSTR.  */
स्थिर YYPTRDIFF_T
yystrlen (ध्रुव वर्ण *yystr)
{
  YYPTRDIFF_T yylen;
  प्रति (yylen = 0; yystr[yylen]; yylen++)
    अनुवर्त;
  प्रत्यागम yylen;
}
# endif
#यदि_अन्तः

#यदि_अपरिभाषितम् yystpcpy
# यदि defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# अन्यथा
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
स्थिर वर्ण *
yystpcpy (वर्ण *yydest, ध्रुव वर्ण *yysrc)
{
  वर्ण *yyd = yydest;
  ध्रुव वर्ण *yys = yysrc;

  यावत् ((*yyd++ = *yys++) != '\0')
    अनुवर्त;

  प्रत्यागम yyd - 1;
}
# endif
#यदि_अन्तः



स्थिर पूर्णाङ्क
yy_syntax_error_arguments (ध्रुव yypcontext_t *yyctx,
                           yysymbol_विधा_t yyarg[], पूर्णाङ्क yyargn)
{
  /* Actual size of YYARG. */
  पूर्णाङ्क yycount = 0;
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
  यदि (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      पूर्णाङ्क yyn;
      यदि (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      यदि (yyn == YYENOMEM)
        प्रत्यागम YYENOMEM;
      अन्यथा
        yycount += yyn;
    }
  प्रत्यागम yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
स्थिर पूर्णाङ्क
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, वर्ण **yymsg,
                ध्रुव yypcontext_t *yyctx)
{
  गणना { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  ध्रुव वर्ण *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_विधा_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  पूर्णाङ्क yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  यदि (yycount == YYENOMEM)
    प्रत्यागम YYENOMEM;

  विकल्प (yycount)
    {
#परिभाषय YYCASE_(N, S)                       \
      पक्ष N:                               \
        yyformat = S;                       \
        भङ्ग
    स्वभाव: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#अपरिभाषय YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    पूर्णाङ्क yyi;
    प्रति (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        यदि (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_अधिकतमम्)
          yysize = yysize1;
        अन्यथा
          प्रत्यागम YYENOMEM;
      }
  }

  यदि (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      यदि (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_अधिकतमम्))
        *yymsg_alloc = YYSTACK_ALLOC_अधिकतमम्;
      प्रत्यागम -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    वर्ण *yyp = *yymsg;
    पूर्णाङ्क yyi = 0;
    यावत् ((*yyp = *yyformat) != '\0')
      यदि (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
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
  प्रत्यागम 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

स्थिर शून्य
yydestruct (ध्रुव वर्ण *yymsg,
            yysymbol_विधा_t yykind, YYSTYPE *yyvaluep, संरचना parser_param *param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  यदि (!yymsg)
    yymsg = "Deleting";
  YY_चिह्नम्_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  विकल्प (yykind)
    {
    पक्ष YYSYMBOL_STRING: /* "string literal"  */
#पङ्क्तिः 259 "src/xkbcomp/parser.y"
            { मुक्त(((*yyvaluep).str)); }
#पङ्क्तिः 1640 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_XkbFile: /* XkbFile  */
#पङ्क्तिः 257 "src/xkbcomp/parser.y"
            { यदि (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#पङ्क्तिः 1646 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Xkb_संयुक्त_Map: /* XkbCompositeMap  */
#पङ्क्तिः 257 "src/xkbcomp/parser.y"
            { यदि (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#पङ्क्तिः 1652 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_XkbMapConfigList: /* XkbMapConfigList  */
#पङ्क्तिः 258 "src/xkbcomp/parser.y"
            { FreeXkbFile(((*yyvaluep).fileList).शीर्ष); }
#पङ्क्तिः 1658 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_XkbMapConfig: /* XkbMapConfig  */
#पङ्क्तिः 257 "src/xkbcomp/parser.y"
            { यदि (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#पङ्क्तिः 1664 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_DeclList: /* DeclList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).anyList).शीर्ष); }
#पङ्क्तिः 1670 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Decl: /* Decl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).any)); }
#पङ्क्तिः 1676 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_VarDecl: /* VarDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#पङ्क्तिः 1682 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_कुञ्चिका_Name_Decl: /* KeyNameDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).कुञ्चिका_संहिता)); }
#पङ्क्तिः 1688 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_कुञ्चिका_Alias_Decl: /* KeyAliasDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).कुञ्चिका_Alias)); }
#पङ्क्तिः 1694 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_VModDecl: /* VModDecl  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).शीर्ष); }
#पङ्क्तिः 1700 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_VModDefList: /* VModDefList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).शीर्ष); }
#पङ्क्तिः 1706 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_VModDef: /* VModDef  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmod)); }
#पङ्क्तिः 1712 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_InterpretDecl: /* InterpretDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#पङ्क्तिः 1718 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Interpret_मेलनम्: /* InterpretMatch  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#पङ्क्तिः 1724 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_VarDeclList: /* VarDeclList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).शीर्ष); }
#पङ्क्तिः 1730 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_कुञ्चिका_Type_Decl: /* KeyTypeDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).कुञ्चिका_Type)); }
#पङ्क्तिः 1736 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_SymbolsDecl: /* SymbolsDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).syms)); }
#पङ्क्तिः 1742 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_OptSymbolsBody: /* OptSymbolsBody  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).शीर्ष); }
#पङ्क्तिः 1748 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_SymbolsBody: /* SymbolsBody  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).शीर्ष); }
#पङ्क्तिः 1754 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_SymbolsVarDecl: /* SymbolsVarDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#पङ्क्तिः 1760 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Multi_कुञ्चिका_Sym_Or_Action_List: /* MultiKeySymOrActionList  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1766 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_गणः_Compat_Decl: /* GroupCompatDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).गणः_Compat)); }
#पङ्क्तिः 1772 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_ModMapDecl: /* ModMapDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).mod_आवरणम्_2)); }
#पङ्क्तिः 1778 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_कुञ्चिका_Or_कुञ्चिका_Sym_List: /* KeyOrKeySymList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 1784 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_कुञ्चिका_Or_कुञ्चिका_Sym: /* KeyOrKeySym  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1790 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_LedMapDecl: /* LedMapDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledMap)); }
#पङ्क्तिः 1796 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_LedNameDecl: /* LedNameDecl  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledName)); }
#पङ्क्तिः 1802 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_CoordList: /* CoordList  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1808 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Coord: /* Coord  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1814 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_व्यञ्जकम्_List: /* ExprList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 1820 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_व्यञ्जकम्: /*व्यञ्जकम्*/
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1826 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_पदांशः: /*पदांशः*/
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1832 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_MultiActionList: /* MultiActionList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 1838 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_ActionList: /* ActionList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 1844 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Non_रिक्तानि_Actions: /* NonEmptyActions  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1850 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Actions: /* Actions  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1856 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Action: /* Action  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1862 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Lhs: /* Lhs  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1868 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_OptTerminal: /* OptTerminal  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1874 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Terminal: /* Terminal  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1880 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Multi_कुञ्चिका_Sym_List: /* MultiKeySymList  */
#पङ्क्तिः 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 1886 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_कुञ्चिका_Sym_List: /* KeySymList  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1892 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_Non_रिक्तानि_कुञ्चिका_Syms: /* NonEmptyKeySyms  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1898 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_कुञ्चिका_Syms: /* KeySyms  */
#पङ्क्तिः 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).व्यञ्जकम्)); }
#पङ्क्तिः 1904 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_OptMapName: /* OptMapName  */
#पङ्क्तिः 259 "src/xkbcomp/parser.y"
            { मुक्त(((*yyvaluep).str)); }
#पङ्क्तिः 1910 "src/xkbcomp/parser.c"
        भङ्ग;

    पक्ष YYSYMBOL_MapName: /* MapName  */
#पङ्क्तिः 259 "src/xkbcomp/parser.y"
            { मुक्त(((*yyvaluep).str)); }
#पङ्क्तिः 1916 "src/xkbcomp/parser.c"
        भङ्ग;

      स्वभाव:
        भङ्ग;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः
}






/*----------.
| yyparse.  |
`----------*/

पूर्णाङ्क
yyparse (संरचना parser_param *param)
{
/* Lookahead token kind.  */
पूर्णाङ्क yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_मानम् (स्थिर YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_मानम् (= yyval_default);

    /* Number of syntax errors so far.  */
    पूर्णाङ्क yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    पूर्णाङ्क yyerrstatus = 0;

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

  पूर्णाङ्क yyn;
  /* The return value of yyparse.  */
  पूर्णाङ्क yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_विधा_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  वर्ण yymsgbuf[128];
  वर्ण *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = परिमाण yymsgbuf;

#परिभाषय YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  पूर्णाङ्क yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  गच्छ yysetstate;


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
  YY_IGNORE_USELESS_CAST_अन्तः
  YY_राशिः_PRINT (yyss, yyssp);

  यदि (yyss + yystacksize - 1 <= yyssp)
#यदि !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#अन्यथा
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# यदि defined yyoverflow
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
      यदि (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      यदि (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        सङ्घ yyalloc *yyptr =
          YY_CAST (सङ्घ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_अष्टकानि (yystacksize))));
        यदि (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        यदि (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (दीर्घ, yystacksize)));
      YY_IGNORE_USELESS_CAST_अन्तः

      यदि (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#यदि_अन्तः /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  यदि (yystate == YYFINAL)
    YYACCEPT;

  गच्छ yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  यदि (yypact_मानम्_is_default (yyn))
    गच्छ yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  यदि (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, param_scanner);
    }

  यदि (yychar <= अन्तः_सम्बन्धि_FILE)
    {
      yychar = अन्तः_सम्बन्धि_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  अन्यथा यदि (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      गच्छ yyerrlab1;
    }
  अन्यथा
    {
      yytoken = YYTRANSLATE (yychar);
      YY_चिह्नम्_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  यदि (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    गच्छ yydefault;
  yyn = yytable[yyn];
  यदि (yyn <= 0)
    {
      यदि (yytable_मानम्_is_error (yyn))
        गच्छ yyerrlab;
      yyn = -yyn;
      गच्छ yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  यदि (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_चिह्नम्_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  गच्छ yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  यदि (yyn == 0)
    गच्छ yyerrlab;
  गच्छ yyreduce;


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
  विकल्प (yyn)
    {
  पक्ष 2: /* XkbFile: XkbCompositeMap  */
#पङ्क्तिः 276 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->अधिकम्_maps = !!param->rtrn; (शून्य) yynerrs; }
#पङ्क्तिः 2195 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 3: /* XkbFile: XkbMapConfig  */
#पङ्क्तिः 278 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->अधिकम्_maps = !!param->rtrn; YYACCEPT; }
#पङ्क्तिः 2201 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 4: /* XkbFile: "end of file"  */
#पङ्क्तिः 280 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = रिक्त; param->अधिकम्_maps = false; }
#पङ्क्तिः 2207 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 5: /* XkbCompositeMap: OptFlags XkbCompositeType OptMapName "{" XkbMapConfigList "}" ";"  */
#पङ्क्तिः 286 "src/xkbcomp/parser.y"
                        { (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (ParseCommon *) (yyvsp[-2].fileList).शीर्ष, (yyvsp[-6].mapFlags)); }
#पङ्क्तिः 2213 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 6: /* XkbCompositeType: "xkb_keymap"  */
#पङ्क्तिः 289 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#पङ्क्तिः 2219 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 7: /* XkbCompositeType: "xkb_semantics"  */
#पङ्क्तिः 290 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#पङ्क्तिः 2225 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 8: /* XkbCompositeType: "xkb_layout"  */
#पङ्क्तिः 291 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#पङ्क्तिः 2231 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 9: /* XkbMapConfigList: XkbMapConfigList XkbMapConfig  */
#पङ्क्तिः 298 "src/xkbcomp/parser.y"
                        {
                            यदि ((yyvsp[0].file)) {
                                यदि ((yyvsp[-1].fileList).शीर्ष) {
                                    (yyval.fileList).शीर्ष = (yyvsp[-1].fileList).शीर्ष;
                                    (yyval.fileList).चरमः->common.अग्र = &(yyvsp[0].file)->common;
                                    (yyval.fileList).चरमः = (yyvsp[0].file);
                                } अन्यथा {
                                    (yyval.fileList).शीर्ष = (yyval.fileList).चरमः = (yyvsp[0].file);
                                }
                            }
                        }
#पङ्क्तिः 2247 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 10: /* XkbMapConfigList: %empty  */
#पङ्क्तिः 309 "src/xkbcomp/parser.y"
                        { (yyval.fileList).शीर्ष = (yyval.fileList).चरमः = रिक्त; }
#पङ्क्तिः 2253 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 11: /* XkbMapConfig: OptFlags FileType OptMapName "{" DeclList "}" ";"  */
#पङ्क्तिः 315 "src/xkbcomp/parser.y"
                        {
                            यदि ((yyvsp[-6].mapFlags) & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            safe_map_name((yyvsp[-4].str)));
                            }
                            (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (yyvsp[-2].anyList).शीर्ष, (yyvsp[-6].mapFlags));
                        }
#पङ्क्तिः 2266 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 12: /* FileType: "xkb_keycodes"  */
#पङ्क्तिः 325 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_KEYCODES; }
#पङ्क्तिः 2272 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 13: /* FileType: "xkb_types"  */
#पङ्क्तिः 326 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_TYPES; }
#पङ्क्तिः 2278 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 14: /* FileType: "xkb_compatibility"  */
#पङ्क्तिः 327 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_COMPAT; }
#पङ्क्तिः 2284 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 15: /* FileType: "xkb_symbols"  */
#पङ्क्तिः 328 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_SYMBOLS; }
#पङ्क्तिः 2290 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 16: /* FileType: "xkb_geometry"  */
#पङ्क्तिः 329 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_GEOMETRY; }
#पङ्क्तिः 2296 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 17: /* OptFlags: Flags  */
#पङ्क्तिः 332 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#पङ्क्तिः 2302 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 18: /* OptFlags: %empty  */
#पङ्क्तिः 333 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = 0; }
#पङ्क्तिः 2308 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 19: /* Flags: Flags Flag  */
#पङ्क्तिः 336 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = ((yyvsp[-1].mapFlags) | (yyvsp[0].mapFlags)); }
#पङ्क्तिः 2314 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 20: /* Flags: Flag  */
#पङ्क्तिः 337 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#पङ्क्तिः 2320 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 21: /* Flag: "partial"  */
#पङ्क्तिः 340 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_PARTIAL; }
#पङ्क्तिः 2326 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 22: /* Flag: "default"  */
#पङ्क्तिः 341 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEFAULT; }
#पङ्क्तिः 2332 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 23: /* Flag: "hidden"  */
#पङ्क्तिः 342 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_HIDDEN; }
#पङ्क्तिः 2338 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 24: /* Flag: "alphanumeric_keys"  */
#पङ्क्तिः 343 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_ALPHANUMERIC; }
#पङ्क्तिः 2344 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 25: /* Flag: "modifier_keys"  */
#पङ्क्तिः 344 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_MODIFIER; }
#पङ्क्तिः 2350 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 26: /* Flag: "keypad_keys"  */
#पङ्क्तिः 345 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_KEYPAD; }
#पङ्क्तिः 2356 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 27: /* Flag: "function_keys"  */
#पङ्क्तिः 346 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_FN; }
#पङ्क्तिः 2362 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 28: /* Flag: "alternate_group"  */
#पङ्क्तिः 347 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_ALTGR; }
#पङ्क्तिः 2368 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 29: /* Flag: "deprecated"  */
#पङ्क्तिः 348 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEPRECATED; }
#पङ्क्तिः 2374 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 30: /* Flag: "identifier"  */
#पङ्क्तिः 350 "src/xkbcomp/parser.y"
                        {
                            ध्रुव bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_संहिता(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_अल्पिष्ठम्,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (अचिह्नित)(yyvsp[0].sval).len, (yyvsp[0].sval).start,
                                (error ? "" : "; ignored")
                            );
                            यदि (error)
                                YYABORT;
                            (yyval.mapFlags) = 0;
                        }
#पङ्क्तिः 2393 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 31: /* DeclList: DeclList Decl  */
#पङ्क्तिः 367 "src/xkbcomp/parser.y"
                        {
                            यदि ((yyvsp[0].any)) {
                                यदि ((yyvsp[-1].anyList).शीर्ष) {
                                    (yyval.anyList).शीर्ष = (yyvsp[-1].anyList).शीर्ष; (yyvsp[-1].anyList).चरमः->अग्र = (yyvsp[0].any); (yyval.anyList).चरमः = (yyvsp[0].any);
                                } अन्यथा {
                                    (yyval.anyList).शीर्ष = (yyval.anyList).चरमः = (yyvsp[0].any);
                                }
                            }
                        }
#पङ्क्तिः 2407 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 32: /* DeclList: DeclList OptMergeMode VModDecl  */
#पङ्क्तिः 382 "src/xkbcomp/parser.y"
                        {
                            प्रति (VModDef *vmod = (yyvsp[0].vmodList).शीर्ष; vmod; vmod = (VModDef *) vmod->common.अग्र)
                                vmod->merge = (yyvsp[-1].merge);
                            यदि ((yyvsp[-2].anyList).शीर्ष) {
                                (yyval.anyList).शीर्ष = (yyvsp[-2].anyList).शीर्ष; (yyvsp[-2].anyList).चरमः->अग्र = &(yyvsp[0].vmodList).शीर्ष->common; (yyval.anyList).चरमः = &(yyvsp[0].vmodList).चरमः->common;
                            } अन्यथा {
                                (yyval.anyList).शीर्ष = &(yyvsp[0].vmodList).शीर्ष->common; (yyval.anyList).चरमः = &(yyvsp[0].vmodList).चरमः->common;
                            }
                        }
#पङ्क्तिः 2421 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 33: /* DeclList: %empty  */
#पङ्क्तिः 391 "src/xkbcomp/parser.y"
                        { (yyval.anyList).शीर्ष = (yyval.anyList).चरमः = रिक्त; }
#पङ्क्तिः 2427 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 34: /* Decl: OptMergeMode VarDecl  */
#पङ्क्तिः 395 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].var)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].var);
                        }
#पङ्क्तिः 2436 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 35: /* Decl: OptMergeMode InterpretDecl  */
#पङ्क्तिः 401 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].interp)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].interp);
                        }
#पङ्क्तिः 2445 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 36: /* Decl: OptMergeMode KeyNameDecl  */
#पङ्क्तिः 406 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].कुञ्चिका_संहिता)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].कुञ्चिका_संहिता);
                        }
#पङ्क्तिः 2454 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 37: /* Decl: OptMergeMode KeyAliasDecl  */
#पङ्क्तिः 411 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].कुञ्चिका_Alias)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].कुञ्चिका_Alias);
                        }
#पङ्क्तिः 2463 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 38: /* Decl: OptMergeMode KeyTypeDecl  */
#पङ्क्तिः 416 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].कुञ्चिका_Type)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].कुञ्चिका_Type);
                        }
#पङ्क्तिः 2472 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 39: /* Decl: OptMergeMode SymbolsDecl  */
#पङ्क्तिः 421 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].syms)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].syms);
                        }
#पङ्क्तिः 2481 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 40: /* Decl: OptMergeMode ModMapDecl  */
#पङ्क्तिः 426 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].mod_आवरणम्_2)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].mod_आवरणम्_2);
                        }
#पङ्क्तिः 2490 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 41: /* Decl: OptMergeMode GroupCompatDecl  */
#पङ्क्तिः 431 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].गणः_Compat)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].गणः_Compat);
                        }
#पङ्क्तिः 2499 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 42: /* Decl: OptMergeMode LedMapDecl  */
#पङ्क्तिः 436 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledMap)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledMap);
                        }
#पङ्क्तिः 2508 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 43: /* Decl: OptMergeMode LedNameDecl  */
#पङ्क्तिः 441 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledName)->merge = (yyvsp[-1].merge);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledName);
                        }
#पङ्क्तिः 2517 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 44: /* Decl: OptMergeMode ShapeDecl  */
#पङ्क्तिः 445 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = रिक्त; }
#पङ्क्तिः 2523 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 45: /* Decl: OptMergeMode SectionDecl  */
#पङ्क्तिः 446 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = रिक्त; }
#पङ्क्तिः 2529 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 46: /* Decl: OptMergeMode DoodadDecl  */
#पङ्क्तिः 447 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = रिक्त; }
#पङ्क्तिः 2535 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 47: /* Decl: OptMergeMode UnknownDecl  */
#पङ्क्तिः 449 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#पङ्क्तिः 2541 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 48: /* Decl: OptMergeMode UnknownCompoundStatementDecl  */
#पङ्क्तिः 451 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#पङ्क्तिः 2547 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 49: /* Decl: MergeMode "string literal"  */
#पङ्क्तिः 453 "src/xkbcomp/parser.y"
                        {
                            (yyval.any) = (ParseCommon *) IncludeCreate(param->ctx, (yyvsp[0].str), (yyvsp[-1].merge));
                            मुक्त((yyvsp[0].str));
                        }
#पङ्क्तिः 2556 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 50: /* VarDecl: Lhs "=" Expr ";"  */
#पङ्क्तिः 460 "src/xkbcomp/parser.y"
                        { (yyval.var) = VarCreate((yyvsp[-3].व्यञ्जकम्), (yyvsp[-1].व्यञ्जकम्)); }
#पङ्क्तिः 2562 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 51: /* VarDecl: Ident ";"  */
#पङ्क्तिः 462 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), true); }
#पङ्क्तिः 2568 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 52: /* VarDecl: "!" Ident ";"  */
#पङ्क्तिः 464 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), false); }
#पङ्क्तिः 2574 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 53: /* KeyNameDecl: "key name" "=" KeyCode ";"  */
#पङ्क्तिः 468 "src/xkbcomp/parser.y"
                        { (yyval.कुञ्चिका_संहिता) = KeycodeCreate((yyvsp[-3].atom), (yyvsp[-1].अङ्कः)); }
#पङ्क्तिः 2580 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 54: /* KeyAliasDecl: "alias" "key name" "=" "key name" ";"  */
#पङ्क्तिः 472 "src/xkbcomp/parser.y"
                        { (yyval.कुञ्चिका_Alias) = कुञ्चिका_Alias_Create((yyvsp[-3].atom), (yyvsp[-1].atom)); }
#पङ्क्तिः 2586 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 55: /* VModDecl: "virtual_modifiers" VModDefList ";"  */
#पङ्क्तिः 476 "src/xkbcomp/parser.y"
                        { (yyval.vmodList) = (yyvsp[-1].vmodList); }
#पङ्क्तिः 2592 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 56: /* VModDefList: VModDefList "," VModDef  */
#पङ्क्तिः 480 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).शीर्ष = (yyvsp[-2].vmodList).शीर्ष; (yyval.vmodList).चरमः->common.अग्र = &(yyvsp[0].vmod)->common; (yyval.vmodList).चरमः = (yyvsp[0].vmod); }
#पङ्क्तिः 2598 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 57: /* VModDefList: VModDef  */
#पङ्क्तिः 482 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).शीर्ष = (yyval.vmodList).चरमः = (yyvsp[0].vmod); }
#पङ्क्तिः 2604 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 58: /* VModDef: Ident  */
#पङ्क्तिः 486 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[0].atom), रिक्त); }
#पङ्क्तिः 2610 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 59: /* VModDef: Ident "=" Expr  */
#पङ्क्तिः 488 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[-2].atom), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 2616 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 60: /* InterpretDecl: "interpret" InterpretMatch "{" VarDeclList "}" ";"  */
#पङ्क्तिः 494 "src/xkbcomp/parser.y"
                        { (yyvsp[-4].interp)->def = (yyvsp[-2].varList).शीर्ष; (yyval.interp) = (yyvsp[-4].interp); }
#पङ्क्तिः 2622 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 61: /* InterpretMatch: KeySym "+" Expr  */
#पङ्क्तिः 498 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[-2].keysym), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 2628 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 62: /* InterpretMatch: KeySym  */
#पङ्क्तिः 500 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[0].keysym), रिक्त); }
#पङ्क्तिः 2634 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 63: /* VarDeclList: VarDeclList VarDecl  */
#पङ्क्तिः 504 "src/xkbcomp/parser.y"
                        {
                            यदि ((yyvsp[0].var)) {
                                यदि ((yyvsp[-1].varList).शीर्ष) {
                                    (yyval.varList).शीर्ष = (yyvsp[-1].varList).शीर्ष;
                                    (yyval.varList).चरमः->common.अग्र = &(yyvsp[0].var)->common;
                                    (yyval.varList).चरमः = (yyvsp[0].var);
                                } अन्यथा {
                                    (yyval.varList).शीर्ष = (yyval.varList).चरमः = (yyvsp[0].var);
                                }
                            }
                        }
#पङ्क्तिः 2650 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 64: /* VarDeclList: %empty  */
#पङ्क्तिः 515 "src/xkbcomp/parser.y"
                        { (yyval.varList).शीर्ष = (yyval.varList).चरमः = रिक्त; }
#पङ्क्तिः 2656 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 65: /* KeyTypeDecl: "type" String "{" VarDeclList "}" ";"  */
#पङ्क्तिः 521 "src/xkbcomp/parser.y"
                        { (yyval.कुञ्चिका_Type) = कुञ्चिका_Type_Create((yyvsp[-4].atom), (yyvsp[-2].varList).शीर्ष); }
#पङ्क्तिः 2662 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 66: /* SymbolsDecl: "key" "key name" "{" OptSymbolsBody "}" ";"  */
#पङ्क्तिः 527 "src/xkbcomp/parser.y"
                        { (yyval.syms) = SymbolsCreate((yyvsp[-4].atom), (yyvsp[-2].varList).शीर्ष); }
#पङ्क्तिः 2668 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 67: /* OptSymbolsBody: SymbolsBody  */
#पङ्क्तिः 530 "src/xkbcomp/parser.y"
                                    { (yyval.varList) = (yyvsp[0].varList); }
#पङ्क्तिः 2674 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 68: /* OptSymbolsBody: %empty  */
#पङ्क्तिः 531 "src/xkbcomp/parser.y"
                                    { (yyval.varList).शीर्ष = (yyval.varList).चरमः = रिक्त; }
#पङ्क्तिः 2680 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 69: /* SymbolsBody: SymbolsBody "," SymbolsVarDecl  */
#पङ्क्तिः 535 "src/xkbcomp/parser.y"
                        { (yyval.varList).शीर्ष = (yyvsp[-2].varList).शीर्ष; (yyval.varList).चरमः->common.अग्र = &(yyvsp[0].var)->common; (yyval.varList).चरमः = (yyvsp[0].var); }
#पङ्क्तिः 2686 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 70: /* SymbolsBody: SymbolsVarDecl  */
#पङ्क्तिः 537 "src/xkbcomp/parser.y"
                        { (yyval.varList).शीर्ष = (yyval.varList).चरमः = (yyvsp[0].var); }
#पङ्क्तिः 2692 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 71: /* SymbolsVarDecl: Lhs "=" Expr  */
#पङ्क्तिः 540 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate((yyvsp[-2].व्यञ्जकम्), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 2698 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 72: /* SymbolsVarDecl: Lhs "=" MultiKeySymOrActionList  */
#पङ्क्तिः 541 "src/xkbcomp/parser.y"
                                                           { (yyval.var) = VarCreate((yyvsp[-2].व्यञ्जकम्), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 2704 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 73: /* SymbolsVarDecl: Ident  */
#पङ्क्तिः 542 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), true); }
#पङ्क्तिः 2710 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 74: /* SymbolsVarDecl: "!" Ident  */
#पङ्क्तिः 543 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), false); }
#पङ्क्तिः 2716 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 75: /* SymbolsVarDecl: MultiKeySymOrActionList  */
#पङ्क्तिः 544 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate(रिक्त, (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 2722 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 76: /* MultiKeySymOrActionList: "[" MultiKeySymList "]"  */
#पङ्क्तिः 560 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[-1].व्यञ्जकम्_List).शीर्ष; }
#पङ्क्तिः 2728 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 77: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiKeySymList "]"  */
#पङ्क्तिः 562 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoSymbol */
                            संरचना {व्यञ्जकम्_Def *शीर्ष; व्यञ्जकम्_Def *चरमः;} list = {
                                .शीर्ष = (yyvsp[-1].व्यञ्जकम्_List).शीर्ष, .चरमः = (yyvsp[-1].व्यञ्जकम्_List).चरमः
                            };
                            प्रति (uint32_t k = 0; k < (yyvsp[-3].no_चिह्नम्_Or_Action_List); k++) {
                                व्यञ्जकम्_Def* ध्रुव syms =
                                    व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्);
                                यदि (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.अग्र = &list.शीर्ष->common;
                                list.शीर्ष = syms;
                            }
                            (yyval.व्यञ्जकम्) = list.शीर्ष;
                        }
#पङ्क्तिः 2750 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 78: /* MultiKeySymOrActionList: "[" MultiActionList "]"  */
#पङ्क्तिः 580 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[-1].व्यञ्जकम्_List).शीर्ष; }
#पङ्क्तिः 2756 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 79: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiActionList "]"  */
#पङ्क्तिः 582 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoAction() */
                            संरचना {व्यञ्जकम्_Def *शीर्ष; व्यञ्जकम्_Def *चरमः;} list = {
                                .शीर्ष = (yyvsp[-1].व्यञ्जकम्_List).शीर्ष, .चरमः = (yyvsp[-1].व्यञ्जकम्_List).चरमः
                            };
                            प्रति (uint32_t k = 0; k < (yyvsp[-3].no_चिह्नम्_Or_Action_List); k++) {
                                व्यञ्जकम्_Def* ध्रुव acts = व्यञ्जकम्_Create_Action_List(रिक्त);
                                यदि (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.अग्र = &list.शीर्ष->common;
                                list.शीर्ष = acts;
                            }
                            (yyval.व्यञ्जकम्) = list.शीर्ष;
                        }
#पङ्क्तिः 2777 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 80: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "]"  */
#पङ्क्तिः 604 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_रिक्तानि_List(); }
#पङ्क्तिः 2783 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 81: /* NoSymbolOrActionList: NoSymbolOrActionList "," "{" "}"  */
#पङ्क्तिः 610 "src/xkbcomp/parser.y"
                        { (yyval.no_चिह्नम्_Or_Action_List) = (yyvsp[-3].no_चिह्नम्_Or_Action_List) + 1; }
#पङ्क्तिः 2789 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 82: /* NoSymbolOrActionList: "{" "}"  */
#पङ्क्तिः 612 "src/xkbcomp/parser.y"
                        { (yyval.no_चिह्नम्_Or_Action_List) = 1; }
#पङ्क्तिः 2795 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 83: /* NoSymbolOrActionList: %empty  */
#पङ्क्तिः 613 "src/xkbcomp/parser.y"
                        { (yyval.no_चिह्नम्_Or_Action_List) = 0; }
#पङ्क्तिः 2801 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 84: /* GroupCompatDecl: "group" Integer "=" Expr ";"  */
#पङ्क्तिः 617 "src/xkbcomp/parser.y"
                        { (yyval.गणः_Compat) = गणः_Compat_Create((yyvsp[-3].अङ्कः), (yyvsp[-1].व्यञ्जकम्)); }
#पङ्क्तिः 2807 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 85: /* ModMapDecl: "modifier_map" Expr "{" KeyOrKeySymList "}" ";"  */
#पङ्क्तिः 621 "src/xkbcomp/parser.y"
                        {
                            यदि (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                (yyvsp[-4].व्यञ्जकम्)->common.type != STMT_व्यञ्जकम्_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_आवरणम्,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) (yyvsp[-4].व्यञ्जकम्));
                                    FreeStmt((ParseCommon *) (yyvsp[-2].व्यञ्जकम्_List).शीर्ष);
                                    YYERROR;
                            }
                            (yyval.mod_आवरणम्_2) = ModMapCreate((yyvsp[-4].व्यञ्जकम्), (yyvsp[-2].व्यञ्जकम्_List).शीर्ष);
                        }
#पङ्क्तिः 2826 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 86: /* KeyOrKeySymList: KeyOrKeySymList "," KeyOrKeySym  */
#पङ्क्तिः 638 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyvsp[-2].व्यञ्जकम्_List).शीर्ष; (yyval.व्यञ्जकम्_List).चरमः->common.अग्र = &(yyvsp[0].व्यञ्जकम्)->common; (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 2832 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 87: /* KeyOrKeySymList: KeyOrKeySym  */
#पङ्क्तिः 640 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 2838 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 88: /* KeyOrKeySym: "key name"  */
#पङ्क्तिः 644 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_कुञ्चिका_Name((yyvsp[0].atom)); }
#पङ्क्तिः 2844 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 89: /* KeyOrKeySym: KeySym  */
#पङ्क्तिः 646 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_कुञ्चिका_Sym((yyvsp[0].keysym)); }
#पङ्क्तिः 2850 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 90: /* LedMapDecl: "indicator" String "{" VarDeclList "}" ";"  */
#पङ्क्तिः 650 "src/xkbcomp/parser.y"
                        { (yyval.ledMap) = LedMapCreate((yyvsp[-4].atom), (yyvsp[-2].varList).शीर्ष); }
#पङ्क्तिः 2856 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 91: /* LedNameDecl: "indicator" Integer "=" Expr ";"  */
#पङ्क्तिः 654 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].अङ्कः), (yyvsp[-1].व्यञ्जकम्), false); }
#पङ्क्तिः 2862 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 92: /* LedNameDecl: "virtual" "indicator" Integer "=" Expr ";"  */
#पङ्क्तिः 656 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].अङ्कः), (yyvsp[-1].व्यञ्जकम्), true); }
#पङ्क्तिः 2868 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 93: /* UnknownDecl: "identifier" Terminal "=" Expr ";"  */
#पङ्क्तिः 660 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-3].व्यञ्जकम्));
                            FreeStmt((ParseCommon *) (yyvsp[-1].व्यञ्जकम्));
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, (yyvsp[-4].sval));
                        }
#पङ्क्तिः 2878 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 94: /* UnknownCompoundStatementDecl: "identifier" OptTerminal "{" VarDeclList "}" ";"  */
#पङ्क्तिः 669 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-4].व्यञ्जकम्));
                            FreeStmt((ParseCommon *) (yyvsp[-2].varList).शीर्ष);
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, (yyvsp[-5].sval));
                        }
#पङ्क्तिः 2888 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 95: /* ShapeDecl: "shape" String "{" OutlineList "}" ";"  */
#पङ्क्तिः 677 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2894 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 96: /* ShapeDecl: "shape" String "{" CoordList "}" ";"  */
#पङ्क्तिः 679 "src/xkbcomp/parser.y"
                        { (शून्य) (yyvsp[-2].व्यञ्जकम्); (yyval.geom) = रिक्त; }
#पङ्क्तिः 2900 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 97: /* SectionDecl: "section" String "{" SectionBody "}" ";"  */
#पङ्क्तिः 683 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2906 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 98: /* SectionBody: SectionBody SectionBodyItem  */
#पङ्क्तिः 686 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = रिक्त;}
#पङ्क्तिः 2912 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 99: /* SectionBody: SectionBodyItem  */
#पङ्क्तिः 687 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2918 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 100: /* SectionBodyItem: "row" "{" RowBody "}" ";"  */
#पङ्क्तिः 691 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2924 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 101: /* SectionBodyItem: VarDecl  */
#पङ्क्तिः 693 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = रिक्त; }
#पङ्क्तिः 2930 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 102: /* SectionBodyItem: DoodadDecl  */
#पङ्क्तिः 695 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2936 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 103: /* SectionBodyItem: LedMapDecl  */
#पङ्क्तिः 697 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ledMap)); (yyval.geom) = रिक्त; }
#पङ्क्तिः 2942 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 104: /* SectionBodyItem: OverlayDecl  */
#पङ्क्तिः 699 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2948 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 105: /* RowBody: RowBody RowBodyItem  */
#पङ्क्तिः 702 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = रिक्त;}
#पङ्क्तिः 2954 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 106: /* RowBody: RowBodyItem  */
#पङ्क्तिः 703 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2960 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 107: /* RowBodyItem: "keys" "{" Keys "}" ";"  */
#पङ्क्तिः 706 "src/xkbcomp/parser.y"
                                                     { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2966 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 108: /* RowBodyItem: VarDecl  */
#पङ्क्तिः 708 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = रिक्त; }
#पङ्क्तिः 2972 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 109: /* Keys: Keys "," Key  */
#पङ्क्तिः 711 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2978 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 110: /* Keys: Key  */
#पङ्क्तिः 712 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2984 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 111: /* Key: "key name"  */
#पङ्क्तिः 716 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 2990 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 112: /* Key: "{" ExprList "}"  */
#पङ्क्तिः 718 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-1].व्यञ्जकम्_List).शीर्ष); (yyval.geom) = रिक्त; }
#पङ्क्तिः 2996 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 113: /* OverlayDecl: "overlay" String "{" OverlayKeyList "}" ";"  */
#पङ्क्तिः 722 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 3002 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 114: /* OverlayKeyList: OverlayKeyList "," OverlayKey  */
#पङ्क्तिः 725 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 3008 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 115: /* OverlayKeyList: OverlayKey  */
#पङ्क्तिः 726 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 3014 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 116: /* OverlayKey: "key name" "=" "key name"  */
#पङ्क्तिः 729 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 3020 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 117: /* OutlineList: OutlineList "," OutlineInList  */
#पङ्क्तिः 733 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त;}
#पङ्क्तिः 3026 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 118: /* OutlineList: OutlineInList  */
#पङ्क्तिः 735 "src/xkbcomp/parser.y"
                        { (yyval.geom) = रिक्त; }
#पङ्क्तिः 3032 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 119: /* OutlineInList: "{" CoordList "}"  */
#पङ्क्तिः 739 "src/xkbcomp/parser.y"
                        { (शून्य) (yyvsp[-1].व्यञ्जकम्); (yyval.geom) = रिक्त; }
#पङ्क्तिः 3038 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 120: /* OutlineInList: Ident "=" "{" CoordList "}"  */
#पङ्क्तिः 741 "src/xkbcomp/parser.y"
                        { (शून्य) (yyvsp[-1].व्यञ्जकम्); (yyval.geom) = रिक्त; }
#पङ्क्तिः 3044 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 121: /* OutlineInList: Ident "=" Expr  */
#पङ्क्तिः 743 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].व्यञ्जकम्)); (yyval.geom) = रिक्त; }
#पङ्क्तिः 3050 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 122: /* CoordList: CoordList "," Coord  */
#पङ्क्तिः 747 "src/xkbcomp/parser.y"
                        { (शून्य) (yyvsp[-2].व्यञ्जकम्); (शून्य) (yyvsp[0].व्यञ्जकम्); (yyval.व्यञ्जकम्) = रिक्त; }
#पङ्क्तिः 3056 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 123: /* CoordList: Coord  */
#पङ्क्तिः 749 "src/xkbcomp/parser.y"
                        { (शून्य) (yyvsp[0].व्यञ्जकम्); (yyval.व्यञ्जकम्) = रिक्त; }
#पङ्क्तिः 3062 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 124: /* Coord: "[" SignedNumber "," SignedNumber "]"  */
#पङ्क्तिः 753 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = रिक्त; }
#पङ्क्तिः 3068 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 125: /* DoodadDecl: DoodadType String "{" VarDeclList "}" ";"  */
#पङ्क्तिः 757 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-2].varList).शीर्ष); (yyval.geom) = रिक्त; }
#पङ्क्तिः 3074 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 126: /* DoodadType: "text"  */
#पङ्क्तिः 760 "src/xkbcomp/parser.y"
                                { (yyval.अङ्कः) = 0; }
#पङ्क्तिः 3080 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 127: /* DoodadType: "outline"  */
#पङ्क्तिः 761 "src/xkbcomp/parser.y"
                                { (yyval.अङ्कः) = 0; }
#पङ्क्तिः 3086 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 128: /* DoodadType: "solid"  */
#पङ्क्तिः 762 "src/xkbcomp/parser.y"
                                { (yyval.अङ्कः) = 0; }
#पङ्क्तिः 3092 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 129: /* DoodadType: "logo"  */
#पङ्क्तिः 763 "src/xkbcomp/parser.y"
                                { (yyval.अङ्कः) = 0; }
#पङ्क्तिः 3098 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 130: /* FieldSpec: Ident  */
#पङ्क्तिः 766 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#पङ्क्तिः 3104 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 131: /* FieldSpec: Element  */
#पङ्क्तिः 767 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#पङ्क्तिः 3110 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 132: /* Element: "action"  */
#पङ्क्तिः 771 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "action"); }
#पङ्क्तिः 3116 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 133: /* Element: "interpret"  */
#पङ्क्तिः 773 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "interpret"); }
#पङ्क्तिः 3122 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 134: /* Element: "type"  */
#पङ्क्तिः 775 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "type"); }
#पङ्क्तिः 3128 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 135: /* Element: "key"  */
#पङ्क्तिः 777 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "key"); }
#पङ्क्तिः 3134 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 136: /* Element: "group"  */
#पङ्क्तिः 779 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "group"); }
#पङ्क्तिः 3140 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 137: /* Element: "modifier_map"  */
#पङ्क्तिः 781 "src/xkbcomp/parser.y"
                        {(yyval.atom) = xkb_atom_intern_literal(param->ctx, "modifier_map");}
#पङ्क्तिः 3146 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 138: /* Element: "indicator"  */
#पङ्क्तिः 783 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "indicator"); }
#पङ्क्तिः 3152 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 139: /* Element: "shape"  */
#पङ्क्तिः 785 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "shape"); }
#पङ्क्तिः 3158 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 140: /* Element: "row"  */
#पङ्क्तिः 787 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "row"); }
#पङ्क्तिः 3164 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 141: /* Element: "section"  */
#पङ्क्तिः 789 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "section"); }
#पङ्क्तिः 3170 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 142: /* Element: "text"  */
#पङ्क्तिः 791 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "text"); }
#पङ्क्तिः 3176 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 143: /* OptMergeMode: MergeMode  */
#पङ्क्तिः 794 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = (yyvsp[0].merge); }
#पङ्क्तिः 3182 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 144: /* OptMergeMode: %empty  */
#पङ्क्तिः 795 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_DEFAULT; }
#पङ्क्तिः 3188 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 145: /* MergeMode: "include"  */
#पङ्क्तिः 798 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_DEFAULT; }
#पङ्क्तिः 3194 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 146: /* MergeMode: "augment"  */
#पङ्क्तिः 799 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_AUGMENT; }
#पङ्क्तिः 3200 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 147: /* MergeMode: "override"  */
#पङ्क्तिः 800 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_OVERRIDE; }
#पङ्क्तिः 3206 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 148: /* MergeMode: "replace"  */
#पङ्क्तिः 801 "src/xkbcomp/parser.y"
                                        { (yyval.merge) = MERGE_REPLACE; }
#पङ्क्तिः 3212 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 149: /* MergeMode: "alternate"  */
#पङ्क्तिः 803 "src/xkbcomp/parser.y"
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_सन्देशः_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    (yyval.merge) = MERGE_DEFAULT;
                }
#पङ्क्तिः 3226 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 150: /* ExprList: ExprList "," Expr  */
#पङ्क्तिः 815 "src/xkbcomp/parser.y"
                        {
                            यदि ((yyvsp[0].व्यञ्जकम्)) {
                                यदि ((yyvsp[-2].व्यञ्जकम्_List).शीर्ष) {
                                    (yyval.व्यञ्जकम्_List).शीर्ष = (yyvsp[-2].व्यञ्जकम्_List).शीर्ष;
                                    (yyval.व्यञ्जकम्_List).चरमः->common.अग्र = &(yyvsp[0].व्यञ्जकम्)->common;
                                    (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्);
                                } अन्यथा {
                                    (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्);
                                }
                            }
                        }
#पङ्क्तिः 3242 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 151: /* ExprList: Expr  */
#पङ्क्तिः 827 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3248 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 152: /* ExprList: %empty  */
#पङ्क्तिः 828 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = रिक्त; }
#पङ्क्तिः 3254 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 153: /* Expr: Expr "/" Expr  */
#पङ्क्तिः 832 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_DIVIDE, (yyvsp[-2].व्यञ्जकम्), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3260 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 154: /* Expr: Expr "+" Expr  */
#पङ्क्तिः 834 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_सङ्कलय, (yyvsp[-2].व्यञ्जकम्), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3266 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 155: /* Expr: Expr "-" Expr  */
#पङ्क्तिः 836 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_व्यवकलय, (yyvsp[-2].व्यञ्जकम्), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3272 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 156: /* Expr: Expr "*" Expr  */
#पङ्क्तिः 838 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_MULTIPLY, (yyvsp[-2].व्यञ्जकम्), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3278 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 157: /* Expr: Lhs "=" Expr  */
#पङ्क्तिः 840 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_नियोजय, (yyvsp[-2].व्यञ्जकम्), (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3284 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 158: /* Expr: Term  */
#पङ्क्तिः 842 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3290 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 159: /* Term: "-" Term  */
#पङ्क्तिः 846 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_NEGATE, (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3296 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 160: /* Term: "+" Term  */
#पङ्क्तिः 848 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_UNARY_PLUS, (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3302 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 161: /* Term: "!" Term  */
#पङ्क्तिः 850 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_NOT, (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3308 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 162: /* Term: "~" Term  */
#पङ्क्तिः 852 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_INVERT, (yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3314 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 163: /* Term: Lhs  */
#पङ्क्तिः 854 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3320 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 164: /* Term: FieldSpec "(" ExprList ")"  */
#पङ्क्तिः 856 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Action((yyvsp[-3].atom), (yyvsp[-1].व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 3326 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 165: /* Term: Actions  */
#पङ्क्तिः 858 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3332 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 166: /* Term: Terminal  */
#पङ्क्तिः 860 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3338 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 167: /* Term: "(" Expr ")"  */
#पङ्क्तिः 862 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[-1].व्यञ्जकम्); }
#पङ्क्तिः 3344 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 168: /* MultiActionList: MultiActionList "," Action  */
#पङ्क्तिः 866 "src/xkbcomp/parser.y"
                        {
                            व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create_Action_List((yyvsp[0].व्यञ्जकम्));
                            (yyval.व्यञ्जकम्_List) = (yyvsp[-2].व्यञ्जकम्_List);
                            (yyval.व्यञ्जकम्_List).चरमः->common.अग्र = &व्यञ्जकम्->common; (yyval.व्यञ्जकम्_List).चरमः = व्यञ्जकम्;
                        }
#पङ्क्तिः 3354 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 169: /* MultiActionList: MultiActionList "," Actions  */
#पङ्क्तिः 872 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List) = (yyvsp[-2].व्यञ्जकम्_List); (yyval.व्यञ्जकम्_List).चरमः->common.अग्र = &(yyvsp[0].व्यञ्जकम्)->common; (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3360 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 170: /* MultiActionList: Action  */
#पङ्क्तिः 874 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = व्यञ्जकम्_Create_Action_List((yyvsp[0].व्यञ्जकम्)); }
#पङ्क्तिः 3366 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 171: /* MultiActionList: NonEmptyActions  */
#पङ्क्तिः 876 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3372 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 172: /* ActionList: ActionList "," Action  */
#पङ्क्तिः 880 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List) = (yyvsp[-2].व्यञ्जकम्_List); (yyval.व्यञ्जकम्_List).चरमः->common.अग्र = &(yyvsp[0].व्यञ्जकम्)->common; (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3378 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 173: /* ActionList: Action  */
#पङ्क्तिः 882 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3384 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 174: /* NonEmptyActions: "{" ActionList "}"  */
#पङ्क्तिः 886 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Action_List((yyvsp[-1].व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 3390 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 175: /* Actions: NonEmptyActions  */
#पङ्क्तिः 890 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3396 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 176: /* Actions: "{" "}"  */
#पङ्क्तिः 892 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Action_List(रिक्त); }
#पङ्क्तिः 3402 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 177: /* Action: FieldSpec "(" ExprList ")"  */
#पङ्क्तिः 896 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Action((yyvsp[-3].atom), (yyvsp[-1].व्यञ्जकम्_List).शीर्ष); }
#पङ्क्तिः 3408 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 178: /* Lhs: FieldSpec  */
#पङ्क्तिः 900 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Ident((yyvsp[0].atom)); }
#पङ्क्तिः 3414 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 179: /* Lhs: FieldSpec "." FieldSpec  */
#पङ्क्तिः 902 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_क्षेत्रम्_Ref((yyvsp[-2].atom), (yyvsp[0].atom)); }
#पङ्क्तिः 3420 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 180: /* Lhs: FieldSpec "[" Expr "]"  */
#पङ्क्तिः 904 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Array_Ref(XKB_ATOM_NONE, (yyvsp[-3].atom), (yyvsp[-1].व्यञ्जकम्)); }
#पङ्क्तिः 3426 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 181: /* Lhs: FieldSpec "." FieldSpec "[" Expr "]"  */
#पङ्क्तिः 906 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Array_Ref((yyvsp[-5].atom), (yyvsp[-3].atom), (yyvsp[-1].व्यञ्जकम्)); }
#पङ्क्तिः 3432 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 182: /* OptTerminal: Terminal  */
#पङ्क्तिः 910 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3438 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 183: /* OptTerminal: %empty  */
#पङ्क्तिः 911 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = रिक्त; }
#पङ्क्तिः 3444 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 184: /* Terminal: String  */
#पङ्क्तिः 915 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_String((yyvsp[0].atom)); }
#पङ्क्तिः 3450 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 185: /* Terminal: Integer  */
#पङ्क्तिः 917 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Integer((yyvsp[0].अङ्कः)); }
#पङ्क्तिः 3456 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 186: /* Terminal: Float  */
#पङ्क्तिः 919 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_Float(/* Discard $1 */); }
#पङ्क्तिः 3462 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 187: /* Terminal: "key name"  */
#पङ्क्तिः 921 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_कुञ्चिका_Name((yyvsp[0].atom)); }
#पङ्क्तिः 3468 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 188: /* MultiKeySymList: MultiKeySymList "," KeySymLit  */
#पङ्क्तिः 925 "src/xkbcomp/parser.y"
                        {
                            व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create_कुञ्चिका_Sym_List((yyvsp[0].keysym));
                            (yyval.व्यञ्जकम्_List) = (yyvsp[-2].व्यञ्जकम्_List);
                            (yyval.व्यञ्जकम्_List).चरमः->common.अग्र = &व्यञ्जकम्->common; (yyval.व्यञ्जकम्_List).चरमः = व्यञ्जकम्;
                        }
#पङ्क्तिः 3478 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 189: /* MultiKeySymList: MultiKeySymList "," KeySyms  */
#पङ्क्तिः 931 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List) = (yyvsp[-2].व्यञ्जकम्_List); (yyval.व्यञ्जकम्_List).चरमः->common.अग्र = &(yyvsp[0].व्यञ्जकम्)->common; (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3484 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 190: /* MultiKeySymList: KeySymLit  */
#पङ्क्तिः 933 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = व्यञ्जकम्_Create_कुञ्चिका_Sym_List((yyvsp[0].keysym)); }
#पङ्क्तिः 3490 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 191: /* MultiKeySymList: NonEmptyKeySyms  */
#पङ्क्तिः 935 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्_List).शीर्ष = (yyval.व्यञ्जकम्_List).चरमः = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3496 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 192: /* KeySymList: KeySymList "," KeySymLit  */
#पङ्क्तिः 939 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Append_कुञ्चिका_Sym_List((yyvsp[-2].व्यञ्जकम्), (yyvsp[0].keysym)); }
#पङ्क्तिः 3502 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 193: /* KeySymList: KeySymList "," "string literal"  */
#पङ्क्तिः 941 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यञ्जकम्) = व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(param->scanner, (yyvsp[-2].व्यञ्जकम्), (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            यदि (!(yyval.व्यञ्जकम्))
                                YYERROR;
                        }
#पङ्क्तिः 3513 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 194: /* KeySymList: KeySymLit  */
#पङ्क्तिः 948 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_कुञ्चिका_Sym_List((yyvsp[0].keysym));
                            यदि (!(yyval.व्यञ्जकम्))
                                YYERROR;
                        }
#पङ्क्तिः 3523 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 195: /* KeySymList: "string literal"  */
#पङ्क्तिः 954 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्);
                            यदि (!(yyval.व्यञ्जकम्))
                                YYERROR;
                            (yyval.व्यञ्जकम्) = व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(param->scanner, (yyval.व्यञ्जकम्), (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            यदि (!(yyval.व्यञ्जकम्))
                                YYERROR;
                        }
#पङ्क्तिः 3537 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 196: /* NonEmptyKeySyms: "{" KeySymList "}"  */
#पङ्क्तिः 966 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[-1].व्यञ्जकम्); }
#पङ्क्तिः 3543 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 197: /* NonEmptyKeySyms: "string literal"  */
#पङ्क्तिः 968 "src/xkbcomp/parser.y"
                        {
                            (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्);
                            यदि (!(yyval.व्यञ्जकम्))
                                YYERROR;
                            (yyval.व्यञ्जकम्) = व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(param->scanner, (yyval.व्यञ्जकम्), (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            यदि (!(yyval.व्यञ्जकम्))
                                YYERROR;
                        }
#पङ्क्तिः 3557 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 198: /* KeySyms: NonEmptyKeySyms  */
#पङ्क्तिः 980 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = (yyvsp[0].व्यञ्जकम्); }
#पङ्क्तिः 3563 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 199: /* KeySyms: "{" "}"  */
#पङ्क्तिः 982 "src/xkbcomp/parser.y"
                        { (yyval.व्यञ्जकम्) = व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्); }
#पङ्क्तिः 3569 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 200: /* KeySym: KeySymLit  */
#पङ्क्तिः 986 "src/xkbcomp/parser.y"
                        { (yyval.keysym) = (yyvsp[0].keysym); }
#पङ्क्तिः 3575 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 201: /* KeySym: "string literal"  */
#पङ्क्तिः 988 "src/xkbcomp/parser.y"
                        {
                            (yyval.keysym) = KeysymParseString(param->scanner, (yyvsp[0].str));
                            मुक्त((yyvsp[0].str));
                            यदि ((yyval.keysym) == XKB_कुञ्चिका_No_चिह्नम्)
                                YYERROR;
                        }
#पङ्क्तिः 3586 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 202: /* KeySymLit: "identifier"  */
#पङ्क्तिः 997 "src/xkbcomp/parser.y"
                        {
                            यदि (!resolve_keysym(param, (yyvsp[0].sval), &(yyval.keysym))) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (अचिह्नित पूर्णाङ्क) (yyvsp[0].sval).len, (yyvsp[0].sval).start
                                );
                                (yyval.keysym) = XKB_कुञ्चिका_No_चिह्नम्;
                            }
                        }
#पङ्क्तिः 3602 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 203: /* KeySymLit: "section"  */
#पङ्क्तिः 1009 "src/xkbcomp/parser.y"
                                { (yyval.keysym) = XKB_कुञ्चिका_section; }
#पङ्क्तिः 3608 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 204: /* KeySymLit: "decimal digit"  */
#पङ्क्तिः 1011 "src/xkbcomp/parser.y"
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            (yyval.keysym) = XKB_कुञ्चिका_0 + (xkb_keysym_t) (yyvsp[0].अङ्कः);
                        }
#पङ्क्तिः 3621 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 205: /* KeySymLit: "integer literal"  */
#पङ्क्तिः 1020 "src/xkbcomp/parser.y"
                        {
                            यदि ((yyvsp[0].अङ्कः) < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -(yyvsp[0].अङ्कः), (yyvsp[0].अङ्कः)
                                );
                                (yyval.keysym) = XKB_कुञ्चिका_No_चिह्नम्;
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
                                यदि ((yyvsp[0].अङ्कः) <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    (yyval.keysym) = (xkb_keysym_t) (yyvsp[0].अङ्कः);
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        (yyval.keysym), रिक्त, (yyval.keysym), "%#06"PRIx32, "");
                                } अन्यथा {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", (yyvsp[0].अङ्कः), (yyvsp[0].अङ्कः)
                                    );
                                    (yyval.keysym) = XKB_कुञ्चिका_No_चिह्नम्;
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
                                    (yyvsp[0].अङ्कः), (yyvsp[0].अङ्कः)
                                );
                            }
                        }
#पङ्क्तिः 3683 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 206: /* SignedNumber: "-" Number  */
#पङ्क्तिः 1079 "src/xkbcomp/parser.y"
                                        { (yyval.अङ्कः) = -(yyvsp[0].अङ्कः); }
#पङ्क्तिः 3689 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 207: /* SignedNumber: Number  */
#पङ्क्तिः 1080 "src/xkbcomp/parser.y"
                                        { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3695 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 208: /* Number: "float literal"  */
#पङ्क्तिः 1083 "src/xkbcomp/parser.y"
                                      { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3701 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 209: /* Number: "decimal digit"  */
#पङ्क्तिः 1084 "src/xkbcomp/parser.y"
                                      { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3707 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 210: /* Number: "integer literal"  */
#पङ्क्तिः 1085 "src/xkbcomp/parser.y"
                                      { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3713 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 211: /* Float: "float literal"  */
#पङ्क्तिः 1088 "src/xkbcomp/parser.y"
                                { (yyval.अङ्कः) = 0; }
#पङ्क्तिः 3719 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 212: /* Integer: "integer literal"  */
#पङ्क्तिः 1091 "src/xkbcomp/parser.y"
                                      { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3725 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 213: /* Integer: "decimal digit"  */
#पङ्क्तिः 1092 "src/xkbcomp/parser.y"
                                      { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3731 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 214: /* KeyCode: "integer literal"  */
#पङ्क्तिः 1095 "src/xkbcomp/parser.y"
                                      { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3737 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 215: /* KeyCode: "decimal digit"  */
#पङ्क्तिः 1096 "src/xkbcomp/parser.y"
                                      { (yyval.अङ्कः) = (yyvsp[0].अङ्कः); }
#पङ्क्तिः 3743 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 216: /* Ident: "identifier"  */
#पङ्क्तिः 1099 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].sval).start, (yyvsp[0].sval).len); }
#पङ्क्तिः 3749 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 217: /* Ident: "default"  */
#पङ्क्तिः 1100 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "default"); }
#पङ्क्तिः 3755 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 218: /* String: "string literal"  */
#पङ्क्तिः 1103 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].str), वाक्यदैर्घ्यम्((yyvsp[0].str))); मुक्त((yyvsp[0].str)); }
#पङ्क्तिः 3761 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 219: /* OptMapName: MapName  */
#पङ्क्तिः 1106 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#पङ्क्तिः 3767 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 220: /* OptMapName: %empty  */
#पङ्क्तिः 1107 "src/xkbcomp/parser.y"
                                { (yyval.str) = रिक्त; }
#पङ्क्तिः 3773 "src/xkbcomp/parser.c"
    भङ्ग;

  पक्ष 221: /* MapName: "string literal"  */
#पङ्क्तिः 1110 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#पङ्क्तिः 3779 "src/xkbcomp/parser.c"
    भङ्ग;


#पङ्क्तिः 3783 "src/xkbcomp/parser.c"

      स्वभाव: भङ्ग;
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
  YY_चिह्नम्_PRINT ("-> $$ =", YY_CAST (yysymbol_विधा_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ध्रुव पूर्णाङ्क yylhs = yyr1[yyn] - YYNTOKENS;
    ध्रुव पूर्णाङ्क yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  गच्छ yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  यदि (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        वर्ण ध्रुव *yymsgp = YY_("syntax error");
        पूर्णाङ्क yysyntax_error_स्थितिः;
        yysyntax_error_स्थितिः = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        यदि (yysyntax_error_स्थितिः == 0)
          yymsgp = yymsg;
        अन्यथा यदि (yysyntax_error_स्थितिः == -1)
          {
            यदि (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (वर्ण *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            यदि (yymsg)
              {
                yysyntax_error_स्थितिः
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            अन्यथा
              {
                yymsg = yymsgbuf;
                yymsg_alloc = परिमाण yymsgbuf;
                yysyntax_error_स्थितिः = YYENOMEM;
              }
          }
        yyerror (param, yymsgp);
        यदि (yysyntax_error_स्थितिः == YYENOMEM)
          YYNOMEM;
      }
    }

  यदि (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      यदि (yychar <= अन्तः_सम्बन्धि_FILE)
        {
          /* Return failure if at end of input.  */
          यदि (yychar == अन्तः_सम्बन्धि_FILE)
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
  गच्छ yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  यदि (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_राशिः_PRINT (yyss, yyssp);
  yystate = *yyssp;
  गच्छ yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  प्रति (;;)
    {
      yyn = yypact[yystate];
      यदि (!yypact_मानम्_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          यदि (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              यदि (0 < yyn)
                भङ्ग;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      यदि (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_चिह्नम् (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_राशिः_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_अन्तः


  /* Shift the error token.  */
  YY_चिह्नम्_PRINT ("Shifting", YY_ACCESSING_चिह्नम् (yyn), yyvsp, yylsp);

  yystate = yyn;
  गच्छ yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  गच्छ yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  गच्छ yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  गच्छ yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  यदि (yychar != YYEMPTY)
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
  YY_राशिः_PRINT (yyss, yyssp);
  यावत् (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_चिह्नम् (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#यदि_अपरिभाषितम् yyoverflow
  यदि (yyss != yyssa)
    YYSTACK_FREE (yyss);
#यदि_अन्तः
  यदि (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  प्रत्यागम yyresult;
}

#पङ्क्तिः 1113 "src/xkbcomp/parser.y"


/* Parse a specific section */
XkbFile *
parse(संरचना xkb_context *ctx, ध्रुव संरचना parser_keymap_config *config,
      संरचना scanner *scanner, ध्रुव वर्ण *map)
{
    पूर्णाङ्क ret;
    XkbFile *first = रिक्त;
    संरचना parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = रिक्त,
        .config = *config,
        .अधिकम्_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    यावत् ((ret = yyparse(&param)) == 0 && param.अधिकम्_maps) {
        यदि (map) {
            यदि (streq_not_null(map, param.rtrn->name))
                प्रत्यागम param.rtrn;
            अन्यथा
                FreeXkbFile(param.rtrn);
        }
        अन्यथा {
            यदि (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                प्रत्यागम param.rtrn;
            }
            अन्यथा यदि (!first) {
                first = param.rtrn;
            }
            अन्यथा {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = रिक्त;
    }

    यदि (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        प्रत्यागम रिक्त;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    यदि (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    प्रत्यागम first;
}

/* Parse the next section */
bool
parse_अग्र(संरचना xkb_context *ctx,
           ध्रुव संरचना parser_keymap_config *config,
           संरचना scanner *scanner, XkbFile **xkb_file)
{
    पूर्णाङ्क ret;
    संरचना parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = रिक्त,
        .config = *config,
        .अधिकम्_maps = false,
    };

    यदि ((ret = yyparse(&param)) == 0 && param.अधिकम्_maps) {
        *xkb_file = param.rtrn;
        प्रत्यागम true;
    } अन्यथा {
        FreeXkbFile(param.rtrn);
        *xkb_file = रिक्त;
        प्रत्यागम (ret == 0);
    }
}
