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
#ವ್ಯಾಖ್ಯಾನಿಸು YYBISON 30802

/* Bison version string.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYBISON_ಆವೃತ್ತಿ "3.8.2"

/* Skeleton name.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYPURE 1

/* Push parsers.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYPUSH 0

/* Pull parsers.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYPULL 1


/* Substitute the variable and function names.  */
#ವ್ಯಾಖ್ಯಾನಿಸು yyparse         _xkbcommon_parse
#ವ್ಯಾಖ್ಯಾನಿಸು yylex           _xkbcommon_lex
#ವ್ಯಾಖ್ಯಾನಿಸು yyerror         _xkbcommon_error
#ವ್ಯಾಖ್ಯಾನಿಸು yydebug         _xkbcommon_debug
#ವ್ಯಾಖ್ಯಾನಿಸು yynerrs         _xkbcommon_nerrs

/* First part of user prologue.  */
#ಸಾಲು 22 "src/xkbcomp/parser.y"

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/parser-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast-build.h"

ರಚನೆ parser_param {
    ರಚನೆ xkb_context *ctx;
    ರಚನೆ scanner *scanner;
    XkbFile *rtrn;
    ರಚನೆ parser_keymap_config config;
    bool ಇನ್ನಷ್ಟು_maps;
};

#ವ್ಯಾಖ್ಯಾನಿಸು parser_log_with_ಸಂಕೇತ(param, level, verbosity, log_ಸಂದೇಶ_id, fmt, ...)   \
    scanner_log_with_ಸಂಕೇತ((param)->scanner, level, verbosity, log_ಸಂದೇಶ_id, fmt,\
                          ##__VA_ARGS__)

#ವ್ಯಾಖ್ಯಾನಿಸು parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#ವ್ಯಾಖ್ಯಾನಿಸು parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#ವ್ಯಾಖ್ಯಾನಿಸು parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ಸ್ಥಿರ ಶೂನ್ಯ
_xkbcommon_error(ರಚನೆ parser_param *param, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", ಸಂದೇಶ);
}

ಸ್ಥಿರ bool
resolve_keysym(ರಚನೆ parser_param *param, ರಚನೆ sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    ಆದರೆ (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_ಕೀಲಿ_Void_ಚಿಹ್ನೆ;
        ಹಿಂತಿರುಗಿ true;
    }

    /* xkb_keysym_from_name needs a C string. */
    ಅಕ್ಷರ buf[XKB_KEYSYM_NAME_MAX_ಗಾತ್ರ];
    ಆದರೆ (name.len >= ಗಾತ್ರ(buf)) {
        ಹಿಂತಿರುಗಿ false;
    }
    ಸ್ಮರಣೆ_ನಕಲು(buf, name.ಆರಂಭ, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    ಆದರೆ (sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        ಹಿಂತಿರುಗಿ true;
    }

    ಹಿಂತಿರುಗಿ false;
}

#ವ್ಯಾಖ್ಯಾನಿಸು param_scanner param->scanner

#ಸಾಲು 150 "src/xkbcomp/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  ಇಲ್ಲದಿದ್ದರೆ
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  ಆದರೆ defined __cplusplus
#   ಆದರೆ 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   ಇಲ್ಲದಿದ್ದರೆ
#    define YY_NULLPTR 0
#   endif
#  ಇಲ್ಲದಿದ್ದರೆ
#   define YY_NULLPTR ((ಶೂನ್ಯ*)0)
#  endif
# endif

#ಅನ್ತರ್ಭಾವ "parser.h"
/* Symbol kind.  */
ಎಣಿಕೆ yysymbol_ಬಗೆ_t
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
  YYSYMBOL_ಕೀಲಿ = 21,                       /* "key"  */
  YYSYMBOL_ALIAS = 22,                     /* "alias"  */
  YYSYMBOL_ಗುಂಪು = 23,                     /* "group"  */
  YYSYMBOL_MODIFIER_MAP = 24,              /* "modifier_map"  */
  YYSYMBOL_INDICATOR = 25,                 /* "indicator"  */
  YYSYMBOL_SHAPE = 26,                     /* "shape"  */
  YYSYMBOL_ಕೀಲಿಗಳು = 27,                      /* "keys"  */
  YYSYMBOL_ಅಡ್ಡಸಾಲು = 28,                       /* "row"  */
  YYSYMBOL_SECTION = 29,                   /* "section"  */
  YYSYMBOL_OVERLAY = 30,                   /* "overlay"  */
  YYSYMBOL_ಪಠ್ಯ = 31,                      /* "text"  */
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
  YYSYMBOL_ALPHANUMERIC_ಕೀಲಿಗಳು = 61,         /* "alphanumeric_keys"  */
  YYSYMBOL_MODIFIER_ಕೀಲಿಗಳು = 62,             /* "modifier_keys"  */
  YYSYMBOL_KEYPAD_ಕೀಲಿಗಳು = 63,               /* "keypad_keys"  */
  YYSYMBOL_FUNCTION_ಕೀಲಿಗಳು = 64,             /* "function_keys"  */
  YYSYMBOL_ALTERNATE_ಗುಂಪು = 65,           /* "alternate_group"  */
  YYSYMBOL_DEPRECATED = 66,                /* "deprecated"  */
  YYSYMBOL_YYACCEPT = 67,                  /* $accept  */
  YYSYMBOL_XkbFile = 68,                   /* XkbFile  */
  YYSYMBOL_Xkb_ಸಂಯುಕ್ತ_Map = 69,           /* XkbCompositeMap  */
  YYSYMBOL_Xkb_ಸಂಯುಕ್ತ_Type = 70,          /* XkbCompositeType  */
  YYSYMBOL_XkbMapConfigList = 71,          /* XkbMapConfigList  */
  YYSYMBOL_XkbMapConfig = 72,              /* XkbMapConfig  */
  YYSYMBOL_FileType = 73,                  /* FileType  */
  YYSYMBOL_OptFlags = 74,                  /* OptFlags  */
  YYSYMBOL_Flags = 75,                     /* Flags  */
  YYSYMBOL_Flag = 76,                      /* Flag  */
  YYSYMBOL_DeclList = 77,                  /* DeclList  */
  YYSYMBOL_Decl = 78,                      /* Decl  */
  YYSYMBOL_VarDecl = 79,                   /* VarDecl  */
  YYSYMBOL_ಕೀಲಿ_Name_Decl = 80,               /* KeyNameDecl  */
  YYSYMBOL_ಕೀಲಿ_Alias_Decl = 81,              /* KeyAliasDecl  */
  YYSYMBOL_VModDecl = 82,                  /* VModDecl  */
  YYSYMBOL_VModDefList = 83,               /* VModDefList  */
  YYSYMBOL_VModDef = 84,                   /* VModDef  */
  YYSYMBOL_InterpretDecl = 85,             /* InterpretDecl  */
  YYSYMBOL_Interpret_ಹೊಂದಿಕೆ = 86,            /* InterpretMatch  */
  YYSYMBOL_VarDeclList = 87,               /* VarDeclList  */
  YYSYMBOL_ಕೀಲಿ_Type_Decl = 88,               /* KeyTypeDecl  */
  YYSYMBOL_SymbolsDecl = 89,               /* SymbolsDecl  */
  YYSYMBOL_OptSymbolsBody = 90,            /* OptSymbolsBody  */
  YYSYMBOL_SymbolsBody = 91,               /* SymbolsBody  */
  YYSYMBOL_SymbolsVarDecl = 92,            /* SymbolsVarDecl  */
  YYSYMBOL_Multi_ಕೀಲಿ_Sym_Or_Action_List = 93,   /* MultiKeySymOrActionList  */
  YYSYMBOL_No_ಚಿಹ್ನೆ_Or_Action_List = 94,      /* NoSymbolOrActionList  */
  YYSYMBOL_ಗುಂಪು_Compat_Decl = 95,           /* GroupCompatDecl  */
  YYSYMBOL_ModMapDecl = 96,                /* ModMapDecl  */
  YYSYMBOL_ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List = 97,           /* KeyOrKeySymList  */
  YYSYMBOL_ಕೀಲಿ_Or_ಕೀಲಿ_Sym = 98,               /* KeyOrKeySym  */
  YYSYMBOL_LedMapDecl = 99,                /* LedMapDecl  */
  YYSYMBOL_LedNameDecl = 100,              /* LedNameDecl  */
  YYSYMBOL_UnknownDecl = 101,              /* UnknownDecl  */
  YYSYMBOL_UnknownCompoundStatementDecl = 102, /* UnknownCompoundStatementDecl  */
  YYSYMBOL_ShapeDecl = 103,                /* ShapeDecl  */
  YYSYMBOL_SectionDecl = 104,              /* SectionDecl  */
  YYSYMBOL_SectionBody = 105,              /* SectionBody  */
  YYSYMBOL_Section_Body_ಅಂಶ = 106,          /* SectionBodyItem  */
  YYSYMBOL_ಅಡ್ಡಸಾಲು_Body = 107,                  /* RowBody  */
  YYSYMBOL_ಅಡ್ಡಸಾಲು_Body_ಅಂಶ = 108,              /* RowBodyItem  */
  YYSYMBOL_ಕೀಲಿಗಳು_2 = 109,                     /*ಕೀಲಿಗಳು*/
  YYSYMBOL_ಕೀಲಿ_2 = 110,                      /*ಕೀಲಿ*/
  YYSYMBOL_OverlayDecl = 111,              /* OverlayDecl  */
  YYSYMBOL_Overlay_ಕೀಲಿ_List = 112,           /* OverlayKeyList  */
  YYSYMBOL_Overlay_ಕೀಲಿ = 113,               /* OverlayKey  */
  YYSYMBOL_OutlineList = 114,              /* OutlineList  */
  YYSYMBOL_Outline_ಒಳ_List = 115,            /* OutlineInList  */
  YYSYMBOL_CoordList = 116,                /* CoordList  */
  YYSYMBOL_Coord = 117,                    /* Coord  */
  YYSYMBOL_DoodadDecl = 118,               /* DoodadDecl  */
  YYSYMBOL_DoodadType = 119,               /* DoodadType  */
  YYSYMBOL_ಕ್ಷೇತ್ರ_Spec = 120,                /* FieldSpec  */
  YYSYMBOL_Element = 121,                  /* Element  */
  YYSYMBOL_Opt_ವಿಲೀನ_ಬಹುಲಕ = 122,             /* OptMergeMode  */
  YYSYMBOL_ವಿಲೀನ_ಬಹುಲಕ = 123,                /* MergeMode  */
  YYSYMBOL_ಅಭಿವ್ಯಕ್ತಿ_List = 124,                 /* ExprList  */
  YYSYMBOL_ಅಭಿವ್ಯಕ್ತಿ = 125,                     /*ಅಭಿವ್ಯಕ್ತಿ*/
  YYSYMBOL_ಪದ = 126,                     /*ಪದ*/
  YYSYMBOL_MultiActionList = 127,          /* MultiActionList  */
  YYSYMBOL_ActionList = 128,               /* ActionList  */
  YYSYMBOL_Non_ಖಾಲಿಗಳು_Actions = 129,          /* NonEmptyActions  */
  YYSYMBOL_Actions = 130,                  /* Actions  */
  YYSYMBOL_Action = 131,                   /* Action  */
  YYSYMBOL_Lhs = 132,                      /* Lhs  */
  YYSYMBOL_OptTerminal = 133,              /* OptTerminal  */
  YYSYMBOL_Terminal = 134,                 /* Terminal  */
  YYSYMBOL_Multi_ಕೀಲಿ_Sym_List = 135,          /* MultiKeySymList  */
  YYSYMBOL_ಕೀಲಿ_Sym_List = 136,               /* KeySymList  */
  YYSYMBOL_Non_ಖಾಲಿಗಳು_ಕೀಲಿ_Syms = 137,          /* NonEmptyKeySyms  */
  YYSYMBOL_ಕೀಲಿ_Syms = 138,                  /* KeySyms  */
  YYSYMBOL_ಕೀಲಿ_Sym = 139,                   /* KeySym  */
  YYSYMBOL_ಕೀಲಿ_Sym_Lit = 140,                /* KeySymLit  */
  YYSYMBOL_Signed_ಸಂಖ್ಯೆ = 141,             /* SignedNumber  */
  YYSYMBOL_ಸಂಖ್ಯೆ = 142,                   /*ಸಂಖ್ಯೆ*/
  YYSYMBOL_Float = 143,                    /* Float  */
  YYSYMBOL_Integer = 144,                  /* Integer  */
  YYSYMBOL_ಕೀಲಿ_ಸಂಕೇತ = 145,                  /* KeyCode  */
  YYSYMBOL_Ident = 146,                    /* Ident  */
  YYSYMBOL_String = 147,                   /* String  */
  YYSYMBOL_OptMapName = 148,               /* OptMapName  */
  YYSYMBOL_MapName = 149                   /* MapName  */
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ yysymbol_ಬಗೆ_t yysymbol_ಬಗೆ_t;




#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಗಿಡ್ಡ
# undef ಗಿಡ್ಡ
#ಆದರೆ_ಅಂತ್ಯ

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# ಆದರೆ defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#ಆದರೆ_ಅಂತ್ಯ

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __INT_LEAST8_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __INT_LEAST8_TYPE__ yytype_int8;
#ಅಥವಾಆದರೆ defined YY_STDINT_H
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int_least8_t yytype_int8;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ yytype_int8;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __INT_LEAST16_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __INT_LEAST16_TYPE__ yytype_int16;
#ಅಥವಾಆದರೆ defined YY_STDINT_H
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int_least16_t yytype_int16;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಗಿಡ್ಡ yytype_int16;
#ಆದರೆ_ಅಂತ್ಯ

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __UINT_LEAST8_TYPE__ yytype_uint8;
#ಅಥವಾಆದರೆ (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint_least8_t yytype_uint8;
#ಅಥವಾಆದರೆ !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ yytype_uint8;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಗಿಡ್ಡ yytype_uint8;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __UINT_LEAST16_TYPE__ yytype_uint16;
#ಅಥವಾಆದರೆ (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint_least16_t yytype_uint16;
#ಅಥವಾಆದರೆ !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ yytype_uint16;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ yytype_uint16;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYPTRDIFF_T
# ಆದರೆ defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_ಗರಿಷ್ಠ __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_ಗರಿಷ್ಠ PTRDIFF_MAX
# ಇಲ್ಲದಿದ್ದರೆ
#  define YYPTRDIFF_T ದೀರ್ಘ
#  define YYPTRDIFF_ಗರಿಷ್ಠ LONG_MAX
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined ಗಾತ್ರ_t
#  define YYSIZE_T ಗಾತ್ರ_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T ಗಾತ್ರ_t
# ಇಲ್ಲದಿದ್ದರೆ
#  define YYSIZE_T ಚಿಹ್ನೆರಹಿತ
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು YYSIZE_ಗರಿಷ್ಠ                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_ಗರಿಷ್ಠ < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_ಗರಿಷ್ಠ                         \
            : YY_CAST (YYSIZE_T, -1)))

#ವ್ಯಾಖ್ಯಾನಿಸು YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, ಗಾತ್ರ (X))


/* Stored state numbers (used for stacks). */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ yytype_int16 yy_state_t;

/* State numbers in computations.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ yy_state_fast_t;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_
# ಆದರೆ defined YYENABLE_NLS && YYENABLE_NLS
#  ಆದರೆ ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_ATTRIBUTE_ಶುದ್ಧ
# ಆದರೆ defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_ಶುದ್ಧ __attribute__ ((__pure__))
# ಇಲ್ಲದಿದ್ದರೆ
#  define YY_ATTRIBUTE_ಶುದ್ಧ
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_ATTRIBUTE_ಬಳಸದ
# ಆದರೆ defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_ಬಳಸದ __attribute__ ((__unused__))
# ಇಲ್ಲದಿದ್ದರೆ
#  define YY_ATTRIBUTE_ಬಳಸದ
# endif
#ಆದರೆ_ಅಂತ್ಯ

/* Suppress unused-variable warnings by "using" E.  */
#ಆದರೆ ! defined lint || defined __GNUC__
# define YY_USE(E) ((ಶೂನ್ಯ) (E))
#ಇಲ್ಲದಿದ್ದರೆ
# define YY_USE(E) /*ಖಾಲಿಗಳು*/
#ಆದರೆ_ಅಂತ್ಯ

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#ಆದರೆ defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# ಆದರೆ __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# ಇಲ್ಲದಿದ್ದರೆ
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ      \
    _Pragma ("GCC diagnostic pop")
#ಇಲ್ಲದಿದ್ದರೆ
# define YY_INITIAL_ಮೌಲ್ಯ(ಮೌಲ್ಯ_2) ಮೌಲ್ಯ_2
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_INITIAL_ಮೌಲ್ಯ
# define YY_INITIAL_ಮೌಲ್ಯ(ಮೌಲ್ಯ_2) /* Nothing. */
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_ಕೊನೆ            \
    _Pragma ("GCC diagnostic pop")
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_ಕೊನೆ
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು YY_ASSERT(E) ((ಶೂನ್ಯ) (0 && (E)))

#ಆದರೆ 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  ಆದರೆ YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   ಇಲ್ಲದಿದ್ದರೆ
#    define YYSTACK_ALLOC alloca
#    ಆದರೆ ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
#  define YYSTACK_FREE(Ptr) ಮಾಡು { /*ಖಾಲಿಗಳು*/; } ಆಗ (0)
#  ifndef YYSTACK_ALLOC_ಗರಿಷ್ಠ
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_ಗರಿಷ್ಠ 4032 /* reasonable circa 2006 */
#  endif
# ಇಲ್ಲದಿದ್ದರೆ
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_ಗರಿಷ್ಠ
#   define YYSTACK_ALLOC_ಗರಿಷ್ಠ YYSIZE_ಗರಿಷ್ಠ
#  endif
#  ಆದರೆ (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined ಸ್ಮರಣೆ_ಕೊಡು) \
             && (defined YYFREE || defined ಬಿಡುಗಡೆ)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC ಸ್ಮರಣೆ_ಕೊಡು
#   ಆದರೆ ! defined ಸ್ಮರಣೆ_ಕೊಡು && ! defined EXIT_SUCCESS
ಶೂನ್ಯ *ಸ್ಮರಣೆ_ಕೊಡು (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ಬಿಡುಗಡೆ
#   ಆದರೆ ! defined ಬಿಡುಗಡೆ && ! defined EXIT_SUCCESS
ಶೂನ್ಯ ಬಿಡುಗಡೆ (ಶೂನ್ಯ *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#ಆದರೆ_ಅಂತ್ಯ /* 1 */

#ಆದರೆ (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
ಒಕ್ಕೂಟ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_ಗರಿಷ್ಠ (YYSIZEOF (ಒಕ್ಕೂಟ yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_ಬೈಟುಗಳು(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_ಗರಿಷ್ಠ)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(ರಾಶಿ_alloc, ರಾಶಿ_2)                           \
    ಮಾಡು                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ರಾಶಿ_alloc, ರಾಶಿ_2, yysize);                    \
        ರಾಶಿ_2 = &yyptr->ರಾಶಿ_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ರಾಶಿ_2) + YYSTACK_GAP_ಗರಿಷ್ಠ; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    ಆಗ (0)

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  ಆದರೆ defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, ಎಣಿಕೆ_3) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (ಎಣಿಕೆ_3)) * ಗಾತ್ರ (*(Src)))
#  ಇಲ್ಲದಿದ್ದರೆ
#   define YYCOPY(Dst, Src, ಎಣಿಕೆ_3)              \
      ಮಾಡು                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          ಗಾಗಿ (yyi = 0; yyi < (ಎಣಿಕೆ_3); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      ಆಗ (0)
#  endif
# endif
#ಆದರೆ_ಅಂತ್ಯ /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYLAST   945

/* YYNTOKENS -- Number of terminals.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNNTS  83
/* YYNRULES -- Number of rules.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNRULES  221
/* YYNSTATES -- Number of states.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYNSTATES  386

/* YYMAXUTOK -- Last valid token kind.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYMAXUTOK   257


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_ಬಗೆ_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int8 yytranslate[] =
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

#ಆದರೆ YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yyrline[] =
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
#ಆದರೆ_ಅಂತ್ಯ

/** Accessing symbol of state STATE.  */
#ವ್ಯಾಖ್ಯಾನಿಸು YY_ACCESSING_ಚಿಹ್ನೆ(State) YY_CAST (yysymbol_ಬಗೆ_t, yystos[State])

#ಆದರೆ 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yysymbol_name (yysymbol_ಬಗೆ_t yysymbol) YY_ATTRIBUTE_ಬಳಸದ;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
yysymbol_name (yysymbol_ಬಗೆ_t yysymbol)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ yy_sname[] =
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
  ಹಿಂತಿರುಗಿ yy_sname[yysymbol];
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು YYPACT_NINF (-225)

#ವ್ಯಾಖ್ಯಾನಿಸು yypact_ಮೌಲ್ಯ_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#ವ್ಯಾಖ್ಯಾನಿಸು YYTABLE_NINF (-217)

#ವ್ಯಾಖ್ಯಾನಿಸು yytable_ಮೌಲ್ಯ_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yypact[] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_uint8 yydefact[] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yypgoto[] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yydefgoto[] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yytable[] =
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int16 yycheck[] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_uint8 yystos[] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_uint8 yyr1[] =
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ yytype_int8 yyr2[] =
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


ಎಣಿಕೆ { YYENOMEM = -2 };

#ವ್ಯಾಖ್ಯಾನಿಸು yyerrok         (yyerrstatus = 0)
#ವ್ಯಾಖ್ಯಾನಿಸು yyclearin       (yychar = YYEMPTY)

#ವ್ಯಾಖ್ಯಾನಿಸು YYACCEPT        ಹೋಗು yyacceptlab
#ವ್ಯಾಖ್ಯಾನಿಸು YYABORT         ಹೋಗು yyabortlab
#ವ್ಯಾಖ್ಯಾನಿಸು YYERROR         ಹೋಗು yyerrorlab
#ವ್ಯಾಖ್ಯಾನಿಸು YYNOMEM         ಹೋಗು yyexhaustedlab


#ವ್ಯಾಖ್ಯಾನಿಸು YYRECOVERING()  (!!yyerrstatus)

#ವ್ಯಾಖ್ಯಾನಿಸು YYBACKUP(Token, ಮೌಲ್ಯ_2)                                    \
  ಮಾಡು                                                              \
    ಆದರೆ (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (ಮೌಲ್ಯ_2);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        ಹೋಗು yybackup;                                            \
      }                                                           \
    ಇಲ್ಲದಿದ್ದರೆ                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  ಆಗ (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#ವ್ಯಾಖ್ಯಾನಿಸು YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#ಆದರೆ YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(ವಾದಗಳು_2)                        \
ಮಾಡು {                                            \
  ಆದರೆ (yydebug)                                  \
    YYFPRINTF ವಾದಗಳು_2;                             \
} ಆಗ (0)




# define YY_ಚಿಹ್ನೆ_PRINT(Title, ಬಗೆ_2, ಮೌಲ್ಯ_2, Location)                    \
ಮಾಡು {                                                                      \
  ಆದರೆ (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_ಚಿಹ್ನೆ_print (stderr,                                            \
                  ಬಗೆ_2, ಮೌಲ್ಯ_2, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} ಆಗ (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ಚಿಹ್ನೆ_ಮೌಲ್ಯ_print (FILE *yyo,
                       yysymbol_ಬಗೆ_t yykind, YYSTYPE ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ yyvaluep, ರಚನೆ parser_param *param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  ಆದರೆ (!yyvaluep)
    ಹಿಂತಿರುಗಿ;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ಚಿಹ್ನೆ_print (FILE *yyo,
                 yysymbol_ಬಗೆ_t yykind, YYSTYPE ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ yyvaluep, ರಚನೆ parser_param *param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_ಚಿಹ್ನೆ_ಮೌಲ್ಯ_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_ರಾಶಿ_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  ಗಾಗಿ (; yybottom <= yytop; yybottom++)
    {
      ಪೂರ್ಣಾಂಕ yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_ರಾಶಿ_PRINT(Bottom, ತುದಿ)                            \
ಮಾಡು {                                                            \
  ಆದರೆ (yydebug)                                                  \
    yy_ರಾಶಿ_print ((Bottom), (ತುದಿ));                           \
} ಆಗ (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 ಪೂರ್ಣಾಂಕ yyrule, ರಚನೆ parser_param *param)
{
  ಪೂರ್ಣಾಂಕ yylno = yyrline[yyrule];
  ಪೂರ್ಣಾಂಕ yynrhs = yyr2[yyrule];
  ಪೂರ್ಣಾಂಕ yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  ಗಾಗಿ (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_ಚಿಹ್ನೆ_print (stderr,
                       YY_ACCESSING_ಚಿಹ್ನೆ (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(ನಿಯಮ)          \
ಮಾಡು {                                    \
  ಆದರೆ (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, ನಿಯಮ, param); \
} ಆಗ (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
ಪೂರ್ಣಾಂಕ yydebug;
#ಇಲ್ಲದಿದ್ದರೆ /* !YYDEBUG */
# define YYDPRINTF(ವಾದಗಳು_2) ((ಶೂನ್ಯ) 0)
# define YY_ಚಿಹ್ನೆ_PRINT(Title, ಬಗೆ_2, ಮೌಲ್ಯ_2, Location)
# define YY_ರಾಶಿ_PRINT(Bottom, ತುದಿ)
# define YY_REDUCE_PRINT(ನಿಯಮ)
#ಆದರೆ_ಅಂತ್ಯ /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYINITDEPTH
# define YYINITDEPTH 200
#ಆದರೆ_ಅಂತ್ಯ

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ YYMAXDEPTH
# define YYMAXDEPTH 10000
#ಆದರೆ_ಅಂತ್ಯ


/* Context of a parse error.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  yy_state_t *yyssp;
  yysymbol_ಬಗೆ_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yypcontext_expected_ಟೋಕನುಗಳು (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx,
                            yysymbol_ಬಗೆ_t yyarg[], ಪೂರ್ಣಾಂಕ yyargn)
{
  /* Actual size of YYARG. */
  ಪೂರ್ಣಾಂಕ yycount = 0;
  ಪೂರ್ಣಾಂಕ yyn = yypact[+*yyctx->yyssp];
  ಆದರೆ (!yypact_ಮೌಲ್ಯ_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      ಪೂರ್ಣಾಂಕ yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      ಪೂರ್ಣಾಂಕ yychecklim = YYLAST - yyn + 1;
      ಪೂರ್ಣಾಂಕ yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      ಪೂರ್ಣಾಂಕ yyx;
      ಗಾಗಿ (yyx = yyxbegin; yyx < yyxend; ++yyx)
        ಆದರೆ (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_ಮೌಲ್ಯ_is_error (yytable[yyx + yyn]))
          {
            ಆದರೆ (!yyarg)
              ++yycount;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (yycount == yyargn)
              ಹಿಂತಿರುಗಿ 0;
            ಇಲ್ಲದಿದ್ದರೆ
              yyarg[yycount++] = YY_CAST (yysymbol_ಬಗೆ_t, yyx);
          }
    }
  ಆದರೆ (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  ಹಿಂತಿರುಗಿ yycount;
}




#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ yystrlen
# ಆದರೆ defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (S)))
# ಇಲ್ಲದಿದ್ದರೆ
/* Return the length of YYSTR.  */
ಸ್ಥಿರ YYPTRDIFF_T
yystrlen (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yystr)
{
  YYPTRDIFF_T yylen;
  ಗಾಗಿ (yylen = 0; yystr[yylen]; yylen++)
    ಮುಂದುವರಿ;
  ಹಿಂತಿರುಗಿ yylen;
}
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ yystpcpy
# ಆದರೆ defined __GLIBC__ && defined _STRING_H && defined GNU_ಮೂಲ
#  define yystpcpy stpcpy
# ಇಲ್ಲದಿದ್ದರೆ
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
yystpcpy (ಅಕ್ಷರ *yydest, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yysrc)
{
  ಅಕ್ಷರ *yyd = yydest;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yys = yysrc;

  ಆಗ ((*yyd++ = *yys++) != '\0')
    ಮುಂದುವರಿ;

  ಹಿಂತಿರುಗಿ yyd - 1;
}
# endif
#ಆದರೆ_ಅಂತ್ಯ



ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yy_syntax_error_arguments (ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx,
                           yysymbol_ಬಗೆ_t yyarg[], ಪೂರ್ಣಾಂಕ yyargn)
{
  /* Actual size of YYARG. */
  ಪೂರ್ಣಾಂಕ yycount = 0;
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
  ಆದರೆ (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      ಪೂರ್ಣಾಂಕ yyn;
      ಆದರೆ (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_ಟೋಕನುಗಳು (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      ಆದರೆ (yyn == YYENOMEM)
        ಹಿಂತಿರುಗಿ YYENOMEM;
      ಇಲ್ಲದಿದ್ದರೆ
        yycount += yyn;
    }
  ಹಿಂತಿರುಗಿ yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, ಅಕ್ಷರ **yymsg,
                ಸ್ಥಿರಾಂಕ yypcontext_t *yyctx)
{
  ಎಣಿಕೆ { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_ಬಗೆ_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  ಪೂರ್ಣಾಂಕ yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  ಆದರೆ (yycount == YYENOMEM)
    ಹಿಂತಿರುಗಿ YYENOMEM;

  ಆಯ್ಕೆ (yycount)
    {
#ವ್ಯಾಖ್ಯಾನಿಸು YYCASE_(N, S)                       \
      ಸ್ಥಿತಿ N:                               \
        yyformat = S;                       \
        ನಿಲ್ಲಿಸು
    ಪೂರ್ವನಿಯೋಜಿತ: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    ಪೂರ್ಣಾಂಕ yyi;
    ಗಾಗಿ (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        ಆದರೆ (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_ಗರಿಷ್ಠ)
          yysize = yysize1;
        ಇಲ್ಲದಿದ್ದರೆ
          ಹಿಂತಿರುಗಿ YYENOMEM;
      }
  }

  ಆದರೆ (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      ಆದರೆ (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_ಗರಿಷ್ಠ))
        *yymsg_alloc = YYSTACK_ALLOC_ಗರಿಷ್ಠ;
      ಹಿಂತಿರುಗಿ -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    ಅಕ್ಷರ *yyp = *yymsg;
    ಪೂರ್ಣಾಂಕ yyi = 0;
    ಆಗ ((*yyp = *yyformat) != '\0')
      ಆದರೆ (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ++yyp;
          ++yyformat;
        }
  }
  ಹಿಂತಿರುಗಿ 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
yydestruct (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yymsg,
            yysymbol_ಬಗೆ_t yykind, YYSTYPE *yyvaluep, ರಚನೆ parser_param *param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  ಆದರೆ (!yymsg)
    yymsg = "Deleting";
  YY_ಚಿಹ್ನೆ_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ಆಯ್ಕೆ (yykind)
    {
    ಸ್ಥಿತಿ YYSYMBOL_STRING: /* "string literal"  */
#ಸಾಲು 259 "src/xkbcomp/parser.y"
            { ಬಿಡುಗಡೆ(((*yyvaluep).str)); }
#ಸಾಲು 1640 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_XkbFile: /* XkbFile  */
#ಸಾಲು 257 "src/xkbcomp/parser.y"
            { ಆದರೆ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#ಸಾಲು 1646 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Xkb_ಸಂಯುಕ್ತ_Map: /* XkbCompositeMap  */
#ಸಾಲು 257 "src/xkbcomp/parser.y"
            { ಆದರೆ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#ಸಾಲು 1652 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_XkbMapConfigList: /* XkbMapConfigList  */
#ಸಾಲು 258 "src/xkbcomp/parser.y"
            { FreeXkbFile(((*yyvaluep).fileList).ತಲೆ); }
#ಸಾಲು 1658 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_XkbMapConfig: /* XkbMapConfig  */
#ಸಾಲು 257 "src/xkbcomp/parser.y"
            { ಆದರೆ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#ಸಾಲು 1664 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_DeclList: /* DeclList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).anyList).ತಲೆ); }
#ಸಾಲು 1670 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Decl: /* Decl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).any)); }
#ಸಾಲು 1676 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_VarDecl: /* VarDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#ಸಾಲು 1682 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಕೀಲಿ_Name_Decl: /* KeyNameDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಕೀಲಿ_ಸಂಕೇತ)); }
#ಸಾಲು 1688 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಕೀಲಿ_Alias_Decl: /* KeyAliasDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಕೀಲಿ_Alias)); }
#ಸಾಲು 1694 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_VModDecl: /* VModDecl  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).ತಲೆ); }
#ಸಾಲು 1700 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_VModDefList: /* VModDefList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).ತಲೆ); }
#ಸಾಲು 1706 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_VModDef: /* VModDef  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmod)); }
#ಸಾಲು 1712 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_InterpretDecl: /* InterpretDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#ಸಾಲು 1718 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Interpret_ಹೊಂದಿಕೆ: /* InterpretMatch  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#ಸಾಲು 1724 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_VarDeclList: /* VarDeclList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).ತಲೆ); }
#ಸಾಲು 1730 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಕೀಲಿ_Type_Decl: /* KeyTypeDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಕೀಲಿ_Type)); }
#ಸಾಲು 1736 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_SymbolsDecl: /* SymbolsDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).syms)); }
#ಸಾಲು 1742 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_OptSymbolsBody: /* OptSymbolsBody  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).ತಲೆ); }
#ಸಾಲು 1748 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_SymbolsBody: /* SymbolsBody  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).ತಲೆ); }
#ಸಾಲು 1754 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_SymbolsVarDecl: /* SymbolsVarDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#ಸಾಲು 1760 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Multi_ಕೀಲಿ_Sym_Or_Action_List: /* MultiKeySymOrActionList  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1766 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಗುಂಪು_Compat_Decl: /* GroupCompatDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಗುಂಪು_Compat)); }
#ಸಾಲು 1772 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ModMapDecl: /* ModMapDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).mod_ಮುಖವಾಡ_2)); }
#ಸಾಲು 1778 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List: /* KeyOrKeySymList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 1784 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಕೀಲಿ_Or_ಕೀಲಿ_Sym: /* KeyOrKeySym  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1790 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_LedMapDecl: /* LedMapDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledMap)); }
#ಸಾಲು 1796 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_LedNameDecl: /* LedNameDecl  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledName)); }
#ಸಾಲು 1802 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_CoordList: /* CoordList  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1808 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Coord: /* Coord  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1814 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಅಭಿವ್ಯಕ್ತಿ_List: /* ExprList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 1820 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಅಭಿವ್ಯಕ್ತಿ: /*ಅಭಿವ್ಯಕ್ತಿ*/
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1826 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಪದ: /*ಪದ*/
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1832 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_MultiActionList: /* MultiActionList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 1838 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ActionList: /* ActionList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 1844 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Non_ಖಾಲಿಗಳು_Actions: /* NonEmptyActions  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1850 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Actions: /* Actions  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1856 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Action: /* Action  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1862 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Lhs: /* Lhs  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1868 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_OptTerminal: /* OptTerminal  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1874 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Terminal: /* Terminal  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1880 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Multi_ಕೀಲಿ_Sym_List: /* MultiKeySymList  */
#ಸಾಲು 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 1886 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಕೀಲಿ_Sym_List: /* KeySymList  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1892 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_Non_ಖಾಲಿಗಳು_ಕೀಲಿ_Syms: /* NonEmptyKeySyms  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1898 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_ಕೀಲಿ_Syms: /* KeySyms  */
#ಸಾಲು 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 1904 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_OptMapName: /* OptMapName  */
#ಸಾಲು 259 "src/xkbcomp/parser.y"
            { ಬಿಡುಗಡೆ(((*yyvaluep).str)); }
#ಸಾಲು 1910 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ YYSYMBOL_MapName: /* MapName  */
#ಸಾಲು 259 "src/xkbcomp/parser.y"
            { ಬಿಡುಗಡೆ(((*yyvaluep).str)); }
#ಸಾಲು 1916 "src/xkbcomp/parser.c"
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ
}






/*----------.
| yyparse.  |
`----------*/

ಪೂರ್ಣಾಂಕ
yyparse (ರಚನೆ parser_param *param)
{
/* Lookahead token kind.  */
ಪೂರ್ಣಾಂಕ yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_ಮೌಲ್ಯ (ಸ್ಥಿರ YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_ಮೌಲ್ಯ (= yyval_default);

    /* Number of syntax errors so far.  */
    ಪೂರ್ಣಾಂಕ yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    ಪೂರ್ಣಾಂಕ yyerrstatus = 0;

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

  ಪೂರ್ಣಾಂಕ yyn;
  /* The return value of yyparse.  */
  ಪೂರ್ಣಾಂಕ yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_ಬಗೆ_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  ಅಕ್ಷರ yymsgbuf[128];
  ಅಕ್ಷರ *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = ಗಾತ್ರ yymsgbuf;

#ವ್ಯಾಖ್ಯಾನಿಸು YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  ಪೂರ್ಣಾಂಕ yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  ಹೋಗು yysetstate;


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
  YY_IGNORE_USELESS_CAST_ಕೊನೆ
  YY_ರಾಶಿ_PRINT (yyss, yyssp);

  ಆದರೆ (yyss + yystacksize - 1 <= yyssp)
#ಆದರೆ !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# ಆದರೆ defined yyoverflow
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
# ಇಲ್ಲದಿದ್ದರೆ /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      ಆದರೆ (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      ಆದರೆ (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        ಒಕ್ಕೂಟ yyalloc *yyptr =
          YY_CAST (ಒಕ್ಕೂಟ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_ಬೈಟುಗಳು (yystacksize))));
        ಆದರೆ (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        ಆದರೆ (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (ದೀರ್ಘ, yystacksize)));
      YY_IGNORE_USELESS_CAST_ಕೊನೆ

      ಆದರೆ (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#ಆದರೆ_ಅಂತ್ಯ /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  ಆದರೆ (yystate == YYFINAL)
    YYACCEPT;

  ಹೋಗು yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  ಆದರೆ (yypact_ಮೌಲ್ಯ_is_default (yyn))
    ಹೋಗು yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  ಆದರೆ (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, param_scanner);
    }

  ಆದರೆ (yychar <= ಕೊನೆ_ದ_FILE)
    {
      yychar = ಕೊನೆ_ದ_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      ಹೋಗು yyerrlab1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      yytoken = YYTRANSLATE (yychar);
      YY_ಚಿಹ್ನೆ_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  ಆದರೆ (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    ಹೋಗು yydefault;
  yyn = yytable[yyn];
  ಆದರೆ (yyn <= 0)
    {
      ಆದರೆ (yytable_ಮೌಲ್ಯ_is_error (yyn))
        ಹೋಗು yyerrlab;
      yyn = -yyn;
      ಹೋಗು yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  ಆದರೆ (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_ಚಿಹ್ನೆ_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  ಹೋಗು yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  ಆದರೆ (yyn == 0)
    ಹೋಗು yyerrlab;
  ಹೋಗು yyreduce;


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
  ಆಯ್ಕೆ (yyn)
    {
  ಸ್ಥಿತಿ 2: /* XkbFile: XkbCompositeMap  */
#ಸಾಲು 276 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->ಇನ್ನಷ್ಟು_maps = !!param->rtrn; (ಶೂನ್ಯ) yynerrs; }
#ಸಾಲು 2195 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 3: /* XkbFile: XkbMapConfig  */
#ಸಾಲು 278 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->ಇನ್ನಷ್ಟು_maps = !!param->rtrn; YYACCEPT; }
#ಸಾಲು 2201 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 4: /* XkbFile: "end of file"  */
#ಸಾಲು 280 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = ಖಾಲಿ; param->ಇನ್ನಷ್ಟು_maps = false; }
#ಸಾಲು 2207 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 5: /* XkbCompositeMap: OptFlags XkbCompositeType OptMapName "{" XkbMapConfigList "}" ";"  */
#ಸಾಲು 286 "src/xkbcomp/parser.y"
                        { (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (ParseCommon *) (yyvsp[-2].fileList).ತಲೆ, (yyvsp[-6].mapFlags)); }
#ಸಾಲು 2213 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 6: /* XkbCompositeType: "xkb_keymap"  */
#ಸಾಲು 289 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#ಸಾಲು 2219 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 7: /* XkbCompositeType: "xkb_semantics"  */
#ಸಾಲು 290 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#ಸಾಲು 2225 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 8: /* XkbCompositeType: "xkb_layout"  */
#ಸಾಲು 291 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#ಸಾಲು 2231 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 9: /* XkbMapConfigList: XkbMapConfigList XkbMapConfig  */
#ಸಾಲು 298 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ ((yyvsp[0].file)) {
                                ಆದರೆ ((yyvsp[-1].fileList).ತಲೆ) {
                                    (yyval.fileList).ತಲೆ = (yyvsp[-1].fileList).ತಲೆ;
                                    (yyval.fileList).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].file)->common;
                                    (yyval.fileList).ಕೊನೆಯ = (yyvsp[0].file);
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    (yyval.fileList).ತಲೆ = (yyval.fileList).ಕೊನೆಯ = (yyvsp[0].file);
                                }
                            }
                        }
#ಸಾಲು 2247 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 10: /* XkbMapConfigList: %empty  */
#ಸಾಲು 309 "src/xkbcomp/parser.y"
                        { (yyval.fileList).ತಲೆ = (yyval.fileList).ಕೊನೆಯ = ಖಾಲಿ; }
#ಸಾಲು 2253 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 11: /* XkbMapConfig: OptFlags FileType OptMapName "{" DeclList "}" ";"  */
#ಸಾಲು 315 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ ((yyvsp[-6].mapFlags) & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            ಸುರಕ್ಷಿತ_map_name((yyvsp[-4].str)));
                            }
                            (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (yyvsp[-2].anyList).ತಲೆ, (yyvsp[-6].mapFlags));
                        }
#ಸಾಲು 2266 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 12: /* FileType: "xkb_keycodes"  */
#ಸಾಲು 325 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_KEYCODES; }
#ಸಾಲು 2272 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 13: /* FileType: "xkb_types"  */
#ಸಾಲು 326 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_TYPES; }
#ಸಾಲು 2278 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 14: /* FileType: "xkb_compatibility"  */
#ಸಾಲು 327 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_COMPAT; }
#ಸಾಲು 2284 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 15: /* FileType: "xkb_symbols"  */
#ಸಾಲು 328 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_SYMBOLS; }
#ಸಾಲು 2290 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 16: /* FileType: "xkb_geometry"  */
#ಸಾಲು 329 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_GEOMETRY; }
#ಸಾಲು 2296 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 17: /* OptFlags: Flags  */
#ಸಾಲು 332 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#ಸಾಲು 2302 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 18: /* OptFlags: %empty  */
#ಸಾಲು 333 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = 0; }
#ಸಾಲು 2308 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 19: /* Flags: Flags Flag  */
#ಸಾಲು 336 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = ((yyvsp[-1].mapFlags) | (yyvsp[0].mapFlags)); }
#ಸಾಲು 2314 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 20: /* Flags: Flag  */
#ಸಾಲು 337 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#ಸಾಲು 2320 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 21: /* Flag: "partial"  */
#ಸಾಲು 340 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_PARTIAL; }
#ಸಾಲು 2326 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 22: /* Flag: "default"  */
#ಸಾಲು 341 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEFAULT; }
#ಸಾಲು 2332 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 23: /* Flag: "hidden"  */
#ಸಾಲು 342 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_HIDDEN; }
#ಸಾಲು 2338 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 24: /* Flag: "alphanumeric_keys"  */
#ಸಾಲು 343 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_ALPHANUMERIC; }
#ಸಾಲು 2344 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 25: /* Flag: "modifier_keys"  */
#ಸಾಲು 344 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_MODIFIER; }
#ಸಾಲು 2350 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 26: /* Flag: "keypad_keys"  */
#ಸಾಲು 345 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_KEYPAD; }
#ಸಾಲು 2356 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 27: /* Flag: "function_keys"  */
#ಸಾಲು 346 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_FN; }
#ಸಾಲು 2362 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 28: /* Flag: "alternate_group"  */
#ಸಾಲು 347 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_ALTGR; }
#ಸಾಲು 2368 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 29: /* Flag: "deprecated"  */
#ಸಾಲು 348 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEPRECATED; }
#ಸಾಲು 2374 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 30: /* Flag: "identifier"  */
#ಸಾಲು 350 "src/xkbcomp/parser.y"
                        {
                            ಸ್ಥಿರಾಂಕ bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_ಸಂಕೇತ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (ಚಿಹ್ನೆರಹಿತ)(yyvsp[0].sval).len, (yyvsp[0].sval).ಆರಂಭ,
                                (error ? "" : "; ignored")
                            );
                            ಆದರೆ (error)
                                YYABORT;
                            (yyval.mapFlags) = 0;
                        }
#ಸಾಲು 2393 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 31: /* DeclList: DeclList Decl  */
#ಸಾಲು 367 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ ((yyvsp[0].any)) {
                                ಆದರೆ ((yyvsp[-1].anyList).ತಲೆ) {
                                    (yyval.anyList).ತಲೆ = (yyvsp[-1].anyList).ತಲೆ; (yyvsp[-1].anyList).ಕೊನೆಯ->ಮುಂದಿನ = (yyvsp[0].any); (yyval.anyList).ಕೊನೆಯ = (yyvsp[0].any);
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    (yyval.anyList).ತಲೆ = (yyval.anyList).ಕೊನೆಯ = (yyvsp[0].any);
                                }
                            }
                        }
#ಸಾಲು 2407 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 32: /* DeclList: DeclList OptMergeMode VModDecl  */
#ಸಾಲು 382 "src/xkbcomp/parser.y"
                        {
                            ಗಾಗಿ (VModDef *vmod = (yyvsp[0].vmodList).ತಲೆ; vmod; vmod = (VModDef *) vmod->common.ಮುಂದಿನ)
                                vmod->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            ಆದರೆ ((yyvsp[-2].anyList).ತಲೆ) {
                                (yyval.anyList).ತಲೆ = (yyvsp[-2].anyList).ತಲೆ; (yyvsp[-2].anyList).ಕೊನೆಯ->ಮುಂದಿನ = &(yyvsp[0].vmodList).ತಲೆ->common; (yyval.anyList).ಕೊನೆಯ = &(yyvsp[0].vmodList).ಕೊನೆಯ->common;
                            } ಇಲ್ಲದಿದ್ದರೆ {
                                (yyval.anyList).ತಲೆ = &(yyvsp[0].vmodList).ತಲೆ->common; (yyval.anyList).ಕೊನೆಯ = &(yyvsp[0].vmodList).ಕೊನೆಯ->common;
                            }
                        }
#ಸಾಲು 2421 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 33: /* DeclList: %empty  */
#ಸಾಲು 391 "src/xkbcomp/parser.y"
                        { (yyval.anyList).ತಲೆ = (yyval.anyList).ಕೊನೆಯ = ಖಾಲಿ; }
#ಸಾಲು 2427 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 34: /* Decl: OptMergeMode VarDecl  */
#ಸಾಲು 395 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].var)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].var);
                        }
#ಸಾಲು 2436 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 35: /* Decl: OptMergeMode InterpretDecl  */
#ಸಾಲು 401 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].interp)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].interp);
                        }
#ಸಾಲು 2445 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 36: /* Decl: OptMergeMode KeyNameDecl  */
#ಸಾಲು 406 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ಕೀಲಿ_ಸಂಕೇತ)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ಕೀಲಿ_ಸಂಕೇತ);
                        }
#ಸಾಲು 2454 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 37: /* Decl: OptMergeMode KeyAliasDecl  */
#ಸಾಲು 411 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ಕೀಲಿ_Alias)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ಕೀಲಿ_Alias);
                        }
#ಸಾಲು 2463 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 38: /* Decl: OptMergeMode KeyTypeDecl  */
#ಸಾಲು 416 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ಕೀಲಿ_Type)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ಕೀಲಿ_Type);
                        }
#ಸಾಲು 2472 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 39: /* Decl: OptMergeMode SymbolsDecl  */
#ಸಾಲು 421 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].syms)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].syms);
                        }
#ಸಾಲು 2481 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 40: /* Decl: OptMergeMode ModMapDecl  */
#ಸಾಲು 426 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].mod_ಮುಖವಾಡ_2)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].mod_ಮುಖವಾಡ_2);
                        }
#ಸಾಲು 2490 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 41: /* Decl: OptMergeMode GroupCompatDecl  */
#ಸಾಲು 431 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ಗುಂಪು_Compat)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ಗುಂಪು_Compat);
                        }
#ಸಾಲು 2499 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 42: /* Decl: OptMergeMode LedMapDecl  */
#ಸಾಲು 436 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledMap)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledMap);
                        }
#ಸಾಲು 2508 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 43: /* Decl: OptMergeMode LedNameDecl  */
#ಸಾಲು 441 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledName)->ವಿಲೀನ = (yyvsp[-1].ವಿಲೀನ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledName);
                        }
#ಸಾಲು 2517 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 44: /* Decl: OptMergeMode ShapeDecl  */
#ಸಾಲು 445 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ಖಾಲಿ; }
#ಸಾಲು 2523 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 45: /* Decl: OptMergeMode SectionDecl  */
#ಸಾಲು 446 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ಖಾಲಿ; }
#ಸಾಲು 2529 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 46: /* Decl: OptMergeMode DoodadDecl  */
#ಸಾಲು 447 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ಖಾಲಿ; }
#ಸಾಲು 2535 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 47: /* Decl: OptMergeMode UnknownDecl  */
#ಸಾಲು 449 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#ಸಾಲು 2541 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 48: /* Decl: OptMergeMode UnknownCompoundStatementDecl  */
#ಸಾಲು 451 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#ಸಾಲು 2547 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 49: /* Decl: MergeMode "string literal"  */
#ಸಾಲು 453 "src/xkbcomp/parser.y"
                        {
                            (yyval.any) = (ParseCommon *) IncludeCreate(param->ctx, (yyvsp[0].str), (yyvsp[-1].ವಿಲೀನ));
                            ಬಿಡುಗಡೆ((yyvsp[0].str));
                        }
#ಸಾಲು 2556 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 50: /* VarDecl: Lhs "=" Expr ";"  */
#ಸಾಲು 460 "src/xkbcomp/parser.y"
                        { (yyval.var) = VarCreate((yyvsp[-3].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 2562 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 51: /* VarDecl: Ident ";"  */
#ಸಾಲು 462 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), true); }
#ಸಾಲು 2568 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 52: /* VarDecl: "!" Ident ";"  */
#ಸಾಲು 464 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), false); }
#ಸಾಲು 2574 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 53: /* KeyNameDecl: "key name" "=" KeyCode ";"  */
#ಸಾಲು 468 "src/xkbcomp/parser.y"
                        { (yyval.ಕೀಲಿ_ಸಂಕೇತ) = KeycodeCreate((yyvsp[-3].atom), (yyvsp[-1].ಅಂಕೆ)); }
#ಸಾಲು 2580 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 54: /* KeyAliasDecl: "alias" "key name" "=" "key name" ";"  */
#ಸಾಲು 472 "src/xkbcomp/parser.y"
                        { (yyval.ಕೀಲಿ_Alias) = ಕೀಲಿ_Alias_Create((yyvsp[-3].atom), (yyvsp[-1].atom)); }
#ಸಾಲು 2586 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 55: /* VModDecl: "virtual_modifiers" VModDefList ";"  */
#ಸಾಲು 476 "src/xkbcomp/parser.y"
                        { (yyval.vmodList) = (yyvsp[-1].vmodList); }
#ಸಾಲು 2592 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 56: /* VModDefList: VModDefList "," VModDef  */
#ಸಾಲು 480 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).ತಲೆ = (yyvsp[-2].vmodList).ತಲೆ; (yyval.vmodList).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].vmod)->common; (yyval.vmodList).ಕೊನೆಯ = (yyvsp[0].vmod); }
#ಸಾಲು 2598 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 57: /* VModDefList: VModDef  */
#ಸಾಲು 482 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).ತಲೆ = (yyval.vmodList).ಕೊನೆಯ = (yyvsp[0].vmod); }
#ಸಾಲು 2604 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 58: /* VModDef: Ident  */
#ಸಾಲು 486 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[0].atom), ಖಾಲಿ); }
#ಸಾಲು 2610 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 59: /* VModDef: Ident "=" Expr  */
#ಸಾಲು 488 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[-2].atom), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 2616 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 60: /* InterpretDecl: "interpret" InterpretMatch "{" VarDeclList "}" ";"  */
#ಸಾಲು 494 "src/xkbcomp/parser.y"
                        { (yyvsp[-4].interp)->def = (yyvsp[-2].varList).ತಲೆ; (yyval.interp) = (yyvsp[-4].interp); }
#ಸಾಲು 2622 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 61: /* InterpretMatch: KeySym "+" Expr  */
#ಸಾಲು 498 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[-2].keysym), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 2628 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 62: /* InterpretMatch: KeySym  */
#ಸಾಲು 500 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[0].keysym), ಖಾಲಿ); }
#ಸಾಲು 2634 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 63: /* VarDeclList: VarDeclList VarDecl  */
#ಸಾಲು 504 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ ((yyvsp[0].var)) {
                                ಆದರೆ ((yyvsp[-1].varList).ತಲೆ) {
                                    (yyval.varList).ತಲೆ = (yyvsp[-1].varList).ತಲೆ;
                                    (yyval.varList).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].var)->common;
                                    (yyval.varList).ಕೊನೆಯ = (yyvsp[0].var);
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    (yyval.varList).ತಲೆ = (yyval.varList).ಕೊನೆಯ = (yyvsp[0].var);
                                }
                            }
                        }
#ಸಾಲು 2650 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 64: /* VarDeclList: %empty  */
#ಸಾಲು 515 "src/xkbcomp/parser.y"
                        { (yyval.varList).ತಲೆ = (yyval.varList).ಕೊನೆಯ = ಖಾಲಿ; }
#ಸಾಲು 2656 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 65: /* KeyTypeDecl: "type" String "{" VarDeclList "}" ";"  */
#ಸಾಲು 521 "src/xkbcomp/parser.y"
                        { (yyval.ಕೀಲಿ_Type) = ಕೀಲಿ_Type_Create((yyvsp[-4].atom), (yyvsp[-2].varList).ತಲೆ); }
#ಸಾಲು 2662 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 66: /* SymbolsDecl: "key" "key name" "{" OptSymbolsBody "}" ";"  */
#ಸಾಲು 527 "src/xkbcomp/parser.y"
                        { (yyval.syms) = SymbolsCreate((yyvsp[-4].atom), (yyvsp[-2].varList).ತಲೆ); }
#ಸಾಲು 2668 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 67: /* OptSymbolsBody: SymbolsBody  */
#ಸಾಲು 530 "src/xkbcomp/parser.y"
                                    { (yyval.varList) = (yyvsp[0].varList); }
#ಸಾಲು 2674 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 68: /* OptSymbolsBody: %empty  */
#ಸಾಲು 531 "src/xkbcomp/parser.y"
                                    { (yyval.varList).ತಲೆ = (yyval.varList).ಕೊನೆಯ = ಖಾಲಿ; }
#ಸಾಲು 2680 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 69: /* SymbolsBody: SymbolsBody "," SymbolsVarDecl  */
#ಸಾಲು 535 "src/xkbcomp/parser.y"
                        { (yyval.varList).ತಲೆ = (yyvsp[-2].varList).ತಲೆ; (yyval.varList).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].var)->common; (yyval.varList).ಕೊನೆಯ = (yyvsp[0].var); }
#ಸಾಲು 2686 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 70: /* SymbolsBody: SymbolsVarDecl  */
#ಸಾಲು 537 "src/xkbcomp/parser.y"
                        { (yyval.varList).ತಲೆ = (yyval.varList).ಕೊನೆಯ = (yyvsp[0].var); }
#ಸಾಲು 2692 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 71: /* SymbolsVarDecl: Lhs "=" Expr  */
#ಸಾಲು 540 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate((yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 2698 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 72: /* SymbolsVarDecl: Lhs "=" MultiKeySymOrActionList  */
#ಸಾಲು 541 "src/xkbcomp/parser.y"
                                                           { (yyval.var) = VarCreate((yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 2704 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 73: /* SymbolsVarDecl: Ident  */
#ಸಾಲು 542 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), true); }
#ಸಾಲು 2710 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 74: /* SymbolsVarDecl: "!" Ident  */
#ಸಾಲು 543 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), false); }
#ಸಾಲು 2716 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 75: /* SymbolsVarDecl: MultiKeySymOrActionList  */
#ಸಾಲು 544 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate(ಖಾಲಿ, (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 2722 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 76: /* MultiKeySymOrActionList: "[" MultiKeySymList "]"  */
#ಸಾಲು 560 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ; }
#ಸಾಲು 2728 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 77: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiKeySymList "]"  */
#ಸಾಲು 562 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoSymbol */
                            ರಚನೆ {ಅಭಿವ್ಯಕ್ತಿ_Def *ತಲೆ; ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೊನೆಯ;} list = {
                                .ತಲೆ = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ, .ಕೊನೆಯ = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ
                            };
                            ಗಾಗಿ (uint32_t k = 0; k < (yyvsp[-3].no_ಚಿಹ್ನೆ_Or_Action_List); k++) {
                                ಅಭಿವ್ಯಕ್ತಿ_Def* ಸ್ಥಿರಾಂಕ syms =
                                    ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                                ಆದರೆ (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.ಮುಂದಿನ = &list.ತಲೆ->common;
                                list.ತಲೆ = syms;
                            }
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = list.ತಲೆ;
                        }
#ಸಾಲು 2750 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 78: /* MultiKeySymOrActionList: "[" MultiActionList "]"  */
#ಸಾಲು 580 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ; }
#ಸಾಲು 2756 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 79: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiActionList "]"  */
#ಸಾಲು 582 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoAction() */
                            ರಚನೆ {ಅಭಿವ್ಯಕ್ತಿ_Def *ತಲೆ; ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೊನೆಯ;} list = {
                                .ತಲೆ = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ, .ಕೊನೆಯ = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ
                            };
                            ಗಾಗಿ (uint32_t k = 0; k < (yyvsp[-3].no_ಚಿಹ್ನೆ_Or_Action_List); k++) {
                                ಅಭಿವ್ಯಕ್ತಿ_Def* ಸ್ಥಿರಾಂಕ acts = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಖಾಲಿ);
                                ಆದರೆ (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.ಮುಂದಿನ = &list.ತಲೆ->common;
                                list.ತಲೆ = acts;
                            }
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = list.ತಲೆ;
                        }
#ಸಾಲು 2777 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 80: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "]"  */
#ಸಾಲು 604 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_List(); }
#ಸಾಲು 2783 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 81: /* NoSymbolOrActionList: NoSymbolOrActionList "," "{" "}"  */
#ಸಾಲು 610 "src/xkbcomp/parser.y"
                        { (yyval.no_ಚಿಹ್ನೆ_Or_Action_List) = (yyvsp[-3].no_ಚಿಹ್ನೆ_Or_Action_List) + 1; }
#ಸಾಲು 2789 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 82: /* NoSymbolOrActionList: "{" "}"  */
#ಸಾಲು 612 "src/xkbcomp/parser.y"
                        { (yyval.no_ಚಿಹ್ನೆ_Or_Action_List) = 1; }
#ಸಾಲು 2795 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 83: /* NoSymbolOrActionList: %empty  */
#ಸಾಲು 613 "src/xkbcomp/parser.y"
                        { (yyval.no_ಚಿಹ್ನೆ_Or_Action_List) = 0; }
#ಸಾಲು 2801 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 84: /* GroupCompatDecl: "group" Integer "=" Expr ";"  */
#ಸಾಲು 617 "src/xkbcomp/parser.y"
                        { (yyval.ಗುಂಪು_Compat) = ಗುಂಪು_Compat_Create((yyvsp[-3].ಅಂಕೆ), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 2807 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 85: /* ModMapDecl: "modifier_map" Expr "{" KeyOrKeySymList "}" ";"  */
#ಸಾಲು 621 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ (param->config.format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 &&
                                (yyvsp[-4].ಅಭಿವ್ಯಕ್ತಿ)->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ಮುಖವಾಡ,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) (yyvsp[-4].ಅಭಿವ್ಯಕ್ತಿ));
                                    FreeStmt((ParseCommon *) (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ);
                                    YYERROR;
                            }
                            (yyval.mod_ಮುಖವಾಡ_2) = ModMapCreate((yyvsp[-4].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ);
                        }
#ಸಾಲು 2826 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 86: /* KeyOrKeySymList: KeyOrKeySymList "," KeyOrKeySym  */
#ಸಾಲು 638 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ; (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)->common; (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 2832 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 87: /* KeyOrKeySymList: KeyOrKeySym  */
#ಸಾಲು 640 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 2838 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 88: /* KeyOrKeySym: "key name"  */
#ಸಾಲು 644 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name((yyvsp[0].atom)); }
#ಸಾಲು 2844 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 89: /* KeyOrKeySym: KeySym  */
#ಸಾಲು 646 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym((yyvsp[0].keysym)); }
#ಸಾಲು 2850 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 90: /* LedMapDecl: "indicator" String "{" VarDeclList "}" ";"  */
#ಸಾಲು 650 "src/xkbcomp/parser.y"
                        { (yyval.ledMap) = LedMapCreate((yyvsp[-4].atom), (yyvsp[-2].varList).ತಲೆ); }
#ಸಾಲು 2856 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 91: /* LedNameDecl: "indicator" Integer "=" Expr ";"  */
#ಸಾಲು 654 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].ಅಂಕೆ), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ), false); }
#ಸಾಲು 2862 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 92: /* LedNameDecl: "virtual" "indicator" Integer "=" Expr ";"  */
#ಸಾಲು 656 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].ಅಂಕೆ), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ), true); }
#ಸಾಲು 2868 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 93: /* UnknownDecl: "identifier" Terminal "=" Expr ";"  */
#ಸಾಲು 660 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-3].ಅಭಿವ್ಯಕ್ತಿ));
                            FreeStmt((ParseCommon *) (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ));
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, (yyvsp[-4].sval));
                        }
#ಸಾಲು 2878 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 94: /* UnknownCompoundStatementDecl: "identifier" OptTerminal "{" VarDeclList "}" ";"  */
#ಸಾಲು 669 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-4].ಅಭಿವ್ಯಕ್ತಿ));
                            FreeStmt((ParseCommon *) (yyvsp[-2].varList).ತಲೆ);
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, (yyvsp[-5].sval));
                        }
#ಸಾಲು 2888 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 95: /* ShapeDecl: "shape" String "{" OutlineList "}" ";"  */
#ಸಾಲು 677 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2894 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 96: /* ShapeDecl: "shape" String "{" CoordList "}" ";"  */
#ಸಾಲು 679 "src/xkbcomp/parser.y"
                        { (ಶೂನ್ಯ) (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2900 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 97: /* SectionDecl: "section" String "{" SectionBody "}" ";"  */
#ಸಾಲು 683 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2906 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 98: /* SectionBody: SectionBody SectionBodyItem  */
#ಸಾಲು 686 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ಖಾಲಿ;}
#ಸಾಲು 2912 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 99: /* SectionBody: SectionBodyItem  */
#ಸಾಲು 687 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2918 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 100: /* SectionBodyItem: "row" "{" RowBody "}" ";"  */
#ಸಾಲು 691 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2924 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 101: /* SectionBodyItem: VarDecl  */
#ಸಾಲು 693 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2930 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 102: /* SectionBodyItem: DoodadDecl  */
#ಸಾಲು 695 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2936 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 103: /* SectionBodyItem: LedMapDecl  */
#ಸಾಲು 697 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ledMap)); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2942 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 104: /* SectionBodyItem: OverlayDecl  */
#ಸಾಲು 699 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2948 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 105: /* RowBody: RowBody RowBodyItem  */
#ಸಾಲು 702 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ಖಾಲಿ;}
#ಸಾಲು 2954 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 106: /* RowBody: RowBodyItem  */
#ಸಾಲು 703 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2960 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 107: /* RowBodyItem: "keys" "{" Keys "}" ";"  */
#ಸಾಲು 706 "src/xkbcomp/parser.y"
                                                     { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2966 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 108: /* RowBodyItem: VarDecl  */
#ಸಾಲು 708 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2972 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 109: /* Keys: Keys "," Key  */
#ಸಾಲು 711 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2978 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 110: /* Keys: Key  */
#ಸಾಲು 712 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2984 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 111: /* Key: "key name"  */
#ಸಾಲು 716 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2990 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 112: /* Key: "{" ExprList "}"  */
#ಸಾಲು 718 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 2996 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 113: /* OverlayDecl: "overlay" String "{" OverlayKeyList "}" ";"  */
#ಸಾಲು 722 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3002 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 114: /* OverlayKeyList: OverlayKeyList "," OverlayKey  */
#ಸಾಲು 725 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3008 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 115: /* OverlayKeyList: OverlayKey  */
#ಸಾಲು 726 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3014 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 116: /* OverlayKey: "key name" "=" "key name"  */
#ಸಾಲು 729 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3020 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 117: /* OutlineList: OutlineList "," OutlineInList  */
#ಸಾಲು 733 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ;}
#ಸಾಲು 3026 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 118: /* OutlineList: OutlineInList  */
#ಸಾಲು 735 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3032 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 119: /* OutlineInList: "{" CoordList "}"  */
#ಸಾಲು 739 "src/xkbcomp/parser.y"
                        { (ಶೂನ್ಯ) (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3038 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 120: /* OutlineInList: Ident "=" "{" CoordList "}"  */
#ಸಾಲು 741 "src/xkbcomp/parser.y"
                        { (ಶೂನ್ಯ) (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3044 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 121: /* OutlineInList: Ident "=" Expr  */
#ಸಾಲು 743 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3050 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 122: /* CoordList: CoordList "," Coord  */
#ಸಾಲು 747 "src/xkbcomp/parser.y"
                        { (ಶೂನ್ಯ) (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ); (ಶೂನ್ಯ) (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಖಾಲಿ; }
#ಸಾಲು 3056 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 123: /* CoordList: Coord  */
#ಸಾಲು 749 "src/xkbcomp/parser.y"
                        { (ಶೂನ್ಯ) (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಖಾಲಿ; }
#ಸಾಲು 3062 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 124: /* Coord: "[" SignedNumber "," SignedNumber "]"  */
#ಸಾಲು 753 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಖಾಲಿ; }
#ಸಾಲು 3068 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 125: /* DoodadDecl: DoodadType String "{" VarDeclList "}" ";"  */
#ಸಾಲು 757 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-2].varList).ತಲೆ); (yyval.geom) = ಖಾಲಿ; }
#ಸಾಲು 3074 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 126: /* DoodadType: "text"  */
#ಸಾಲು 760 "src/xkbcomp/parser.y"
                                { (yyval.ಅಂಕೆ) = 0; }
#ಸಾಲು 3080 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 127: /* DoodadType: "outline"  */
#ಸಾಲು 761 "src/xkbcomp/parser.y"
                                { (yyval.ಅಂಕೆ) = 0; }
#ಸಾಲು 3086 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 128: /* DoodadType: "solid"  */
#ಸಾಲು 762 "src/xkbcomp/parser.y"
                                { (yyval.ಅಂಕೆ) = 0; }
#ಸಾಲು 3092 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 129: /* DoodadType: "logo"  */
#ಸಾಲು 763 "src/xkbcomp/parser.y"
                                { (yyval.ಅಂಕೆ) = 0; }
#ಸಾಲು 3098 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 130: /* FieldSpec: Ident  */
#ಸಾಲು 766 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#ಸಾಲು 3104 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 131: /* FieldSpec: Element  */
#ಸಾಲು 767 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#ಸಾಲು 3110 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 132: /* Element: "action"  */
#ಸಾಲು 771 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "action"); }
#ಸಾಲು 3116 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 133: /* Element: "interpret"  */
#ಸಾಲು 773 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "interpret"); }
#ಸಾಲು 3122 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 134: /* Element: "type"  */
#ಸಾಲು 775 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "type"); }
#ಸಾಲು 3128 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 135: /* Element: "key"  */
#ಸಾಲು 777 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "key"); }
#ಸಾಲು 3134 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 136: /* Element: "group"  */
#ಸಾಲು 779 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "group"); }
#ಸಾಲು 3140 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 137: /* Element: "modifier_map"  */
#ಸಾಲು 781 "src/xkbcomp/parser.y"
                        {(yyval.atom) = xkb_atom_intern_literal(param->ctx, "modifier_map");}
#ಸಾಲು 3146 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 138: /* Element: "indicator"  */
#ಸಾಲು 783 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "indicator"); }
#ಸಾಲು 3152 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 139: /* Element: "shape"  */
#ಸಾಲು 785 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "shape"); }
#ಸಾಲು 3158 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 140: /* Element: "row"  */
#ಸಾಲು 787 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "row"); }
#ಸಾಲು 3164 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 141: /* Element: "section"  */
#ಸಾಲು 789 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "section"); }
#ಸಾಲು 3170 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 142: /* Element: "text"  */
#ಸಾಲು 791 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "text"); }
#ಸಾಲು 3176 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 143: /* OptMergeMode: MergeMode  */
#ಸಾಲು 794 "src/xkbcomp/parser.y"
                                        { (yyval.ವಿಲೀನ) = (yyvsp[0].ವಿಲೀನ); }
#ಸಾಲು 3182 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 144: /* OptMergeMode: %empty  */
#ಸಾಲು 795 "src/xkbcomp/parser.y"
                                        { (yyval.ವಿಲೀನ) = ವಿಲೀನ_DEFAULT; }
#ಸಾಲು 3188 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 145: /* MergeMode: "include"  */
#ಸಾಲು 798 "src/xkbcomp/parser.y"
                                        { (yyval.ವಿಲೀನ) = ವಿಲೀನ_DEFAULT; }
#ಸಾಲು 3194 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 146: /* MergeMode: "augment"  */
#ಸಾಲು 799 "src/xkbcomp/parser.y"
                                        { (yyval.ವಿಲೀನ) = ವಿಲೀನ_AUGMENT; }
#ಸಾಲು 3200 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 147: /* MergeMode: "override"  */
#ಸಾಲು 800 "src/xkbcomp/parser.y"
                                        { (yyval.ವಿಲೀನ) = ವಿಲೀನ_OVERRIDE; }
#ಸಾಲು 3206 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 148: /* MergeMode: "replace"  */
#ಸಾಲು 801 "src/xkbcomp/parser.y"
                                        { (yyval.ವಿಲೀನ) = ವಿಲೀನ_REPLACE; }
#ಸಾಲು 3212 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 149: /* MergeMode: "alternate"  */
#ಸಾಲು 803 "src/xkbcomp/parser.y"
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    (yyval.ವಿಲೀನ) = ವಿಲೀನ_DEFAULT;
                }
#ಸಾಲು 3226 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 150: /* ExprList: ExprList "," Expr  */
#ಸಾಲು 815 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ ((yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)) {
                                ಆದರೆ ((yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ) {
                                    (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ;
                                    (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)->common;
                                    (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ);
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ);
                                }
                            }
                        }
#ಸಾಲು 3242 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 151: /* ExprList: Expr  */
#ಸಾಲು 827 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3248 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 152: /* ExprList: %empty  */
#ಸಾಲು 828 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = ಖಾಲಿ; }
#ಸಾಲು 3254 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 153: /* Expr: Expr "/" Expr  */
#ಸಾಲು 832 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE, (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3260 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 154: /* Expr: Expr "+" Expr  */
#ಸಾಲು 834 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು, (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3266 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 155: /* Expr: Expr "-" Expr  */
#ಸಾಲು 836 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ, (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3272 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 156: /* Expr: Expr "*" Expr  */
#ಸಾಲು 838 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY, (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3278 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 157: /* Expr: Lhs "=" Expr  */
#ಸಾಲು 840 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು, (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3284 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 158: /* Expr: Term  */
#ಸಾಲು 842 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3290 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 159: /* Term: "-" Term  */
#ಸಾಲು 846 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE, (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3296 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 160: /* Term: "+" Term  */
#ಸಾಲು 848 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS, (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3302 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 161: /* Term: "!" Term  */
#ಸಾಲು 850 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT, (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3308 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 162: /* Term: "~" Term  */
#ಸಾಲು 852 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT, (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3314 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 163: /* Term: Lhs  */
#ಸಾಲು 854 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3320 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 164: /* Term: FieldSpec "(" ExprList ")"  */
#ಸಾಲು 856 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Action((yyvsp[-3].atom), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 3326 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 165: /* Term: Actions  */
#ಸಾಲು 858 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3332 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 166: /* Term: Terminal  */
#ಸಾಲು 860 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3338 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 167: /* Term: "(" Expr ")"  */
#ಸಾಲು 862 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3344 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 168: /* MultiActionList: MultiActionList "," Action  */
#ಸಾಲು 866 "src/xkbcomp/parser.y"
                        {
                            ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List((yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ));
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ_List) = (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List);
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ->common.ಮುಂದಿನ = &ಅಭಿವ್ಯಕ್ತಿ->common; (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ;
                        }
#ಸಾಲು 3354 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 169: /* MultiActionList: MultiActionList "," Actions  */
#ಸಾಲು 872 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List) = (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List); (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)->common; (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3360 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 170: /* MultiActionList: Action  */
#ಸಾಲು 874 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List((yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3366 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 171: /* MultiActionList: NonEmptyActions  */
#ಸಾಲು 876 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3372 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 172: /* ActionList: ActionList "," Action  */
#ಸಾಲು 880 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List) = (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List); (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)->common; (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3378 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 173: /* ActionList: Action  */
#ಸಾಲು 882 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3384 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 174: /* NonEmptyActions: "{" ActionList "}"  */
#ಸಾಲು 886 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List((yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 3390 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 175: /* Actions: NonEmptyActions  */
#ಸಾಲು 890 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3396 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 176: /* Actions: "{" "}"  */
#ಸಾಲು 892 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಖಾಲಿ); }
#ಸಾಲು 3402 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 177: /* Action: FieldSpec "(" ExprList ")"  */
#ಸಾಲು 896 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Action((yyvsp[-3].atom), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ); }
#ಸಾಲು 3408 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 178: /* Lhs: FieldSpec  */
#ಸಾಲು 900 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Ident((yyvsp[0].atom)); }
#ಸಾಲು 3414 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 179: /* Lhs: FieldSpec "." FieldSpec  */
#ಸಾಲು 902 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕ್ಷೇತ್ರ_Ref((yyvsp[-2].atom), (yyvsp[0].atom)); }
#ಸಾಲು 3420 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 180: /* Lhs: FieldSpec "[" Expr "]"  */
#ಸಾಲು 904 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref(XKB_ATOM_NONE, (yyvsp[-3].atom), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3426 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 181: /* Lhs: FieldSpec "." FieldSpec "[" Expr "]"  */
#ಸಾಲು 906 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref((yyvsp[-5].atom), (yyvsp[-3].atom), (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ)); }
#ಸಾಲು 3432 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 182: /* OptTerminal: Terminal  */
#ಸಾಲು 910 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3438 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 183: /* OptTerminal: %empty  */
#ಸಾಲು 911 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಖಾಲಿ; }
#ಸಾಲು 3444 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 184: /* Terminal: String  */
#ಸಾಲು 915 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_String((yyvsp[0].atom)); }
#ಸಾಲು 3450 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 185: /* Terminal: Integer  */
#ಸಾಲು 917 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Integer((yyvsp[0].ಅಂಕೆ)); }
#ಸಾಲು 3456 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 186: /* Terminal: Float  */
#ಸಾಲು 919 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_Float(/* Discard $1 */); }
#ಸಾಲು 3462 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 187: /* Terminal: "key name"  */
#ಸಾಲು 921 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name((yyvsp[0].atom)); }
#ಸಾಲು 3468 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 188: /* MultiKeySymList: MultiKeySymList "," KeySymLit  */
#ಸಾಲು 925 "src/xkbcomp/parser.y"
                        {
                            ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List((yyvsp[0].keysym));
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ_List) = (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List);
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ->common.ಮುಂದಿನ = &ಅಭಿವ್ಯಕ್ತಿ->common; (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ;
                        }
#ಸಾಲು 3478 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 189: /* MultiKeySymList: MultiKeySymList "," KeySyms  */
#ಸಾಲು 931 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List) = (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ_List); (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ->common.ಮುಂದಿನ = &(yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ)->common; (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3484 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 190: /* MultiKeySymList: KeySymLit  */
#ಸಾಲು 933 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List((yyvsp[0].keysym)); }
#ಸಾಲು 3490 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 191: /* MultiKeySymList: NonEmptyKeySyms  */
#ಸಾಲು 935 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ತಲೆ = (yyval.ಅಭಿವ್ಯಕ್ತಿ_List).ಕೊನೆಯ = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3496 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 192: /* KeySymList: KeySymList "," KeySymLit  */
#ಸಾಲು 939 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Append_ಕೀಲಿ_Sym_List((yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].keysym)); }
#ಸಾಲು 3502 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 193: /* KeySymList: KeySymList "," "string literal"  */
#ಸಾಲು 941 "src/xkbcomp/parser.y"
                        {
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, (yyvsp[-2].ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].str));
                            ಬಿಡುಗಡೆ((yyvsp[0].str));
                            ಆದರೆ (!(yyval.ಅಭಿವ್ಯಕ್ತಿ))
                                YYERROR;
                        }
#ಸಾಲು 3513 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 194: /* KeySymList: KeySymLit  */
#ಸಾಲು 948 "src/xkbcomp/parser.y"
                        {
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List((yyvsp[0].keysym));
                            ಆದರೆ (!(yyval.ಅಭಿವ್ಯಕ್ತಿ))
                                YYERROR;
                        }
#ಸಾಲು 3523 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 195: /* KeySymList: "string literal"  */
#ಸಾಲು 954 "src/xkbcomp/parser.y"
                        {
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                            ಆದರೆ (!(yyval.ಅಭಿವ್ಯಕ್ತಿ))
                                YYERROR;
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, (yyval.ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].str));
                            ಬಿಡುಗಡೆ((yyvsp[0].str));
                            ಆದರೆ (!(yyval.ಅಭಿವ್ಯಕ್ತಿ))
                                YYERROR;
                        }
#ಸಾಲು 3537 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 196: /* NonEmptyKeySyms: "{" KeySymList "}"  */
#ಸಾಲು 966 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[-1].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3543 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 197: /* NonEmptyKeySyms: "string literal"  */
#ಸಾಲು 968 "src/xkbcomp/parser.y"
                        {
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                            ಆದರೆ (!(yyval.ಅಭಿವ್ಯಕ್ತಿ))
                                YYERROR;
                            (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, (yyval.ಅಭಿವ್ಯಕ್ತಿ), (yyvsp[0].str));
                            ಬಿಡುಗಡೆ((yyvsp[0].str));
                            ಆದರೆ (!(yyval.ಅಭಿವ್ಯಕ್ತಿ))
                                YYERROR;
                        }
#ಸಾಲು 3557 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 198: /* KeySyms: NonEmptyKeySyms  */
#ಸಾಲು 980 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = (yyvsp[0].ಅಭಿವ್ಯಕ್ತಿ); }
#ಸಾಲು 3563 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 199: /* KeySyms: "{" "}"  */
#ಸಾಲು 982 "src/xkbcomp/parser.y"
                        { (yyval.ಅಭಿವ್ಯಕ್ತಿ) = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ); }
#ಸಾಲು 3569 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 200: /* KeySym: KeySymLit  */
#ಸಾಲು 986 "src/xkbcomp/parser.y"
                        { (yyval.keysym) = (yyvsp[0].keysym); }
#ಸಾಲು 3575 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 201: /* KeySym: "string literal"  */
#ಸಾಲು 988 "src/xkbcomp/parser.y"
                        {
                            (yyval.keysym) = KeysymParseString(param->scanner, (yyvsp[0].str));
                            ಬಿಡುಗಡೆ((yyvsp[0].str));
                            ಆದರೆ ((yyval.keysym) == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                                YYERROR;
                        }
#ಸಾಲು 3586 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 202: /* KeySymLit: "identifier"  */
#ಸಾಲು 997 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ (!resolve_keysym(param, (yyvsp[0].sval), &(yyval.keysym))) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) (yyvsp[0].sval).len, (yyvsp[0].sval).ಆರಂಭ
                                );
                                (yyval.keysym) = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
                            }
                        }
#ಸಾಲು 3602 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 203: /* KeySymLit: "section"  */
#ಸಾಲು 1009 "src/xkbcomp/parser.y"
                                { (yyval.keysym) = XKB_ಕೀಲಿ_section; }
#ಸಾಲು 3608 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 204: /* KeySymLit: "decimal digit"  */
#ಸಾಲು 1011 "src/xkbcomp/parser.y"
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            (yyval.keysym) = XKB_ಕೀಲಿ_0 + (xkb_keysym_t) (yyvsp[0].ಅಂಕೆ);
                        }
#ಸಾಲು 3621 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 205: /* KeySymLit: "integer literal"  */
#ಸಾಲು 1020 "src/xkbcomp/parser.y"
                        {
                            ಆದರೆ ((yyvsp[0].ಅಂಕೆ) < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -(yyvsp[0].ಅಂಕೆ), (yyvsp[0].ಅಂಕೆ)
                                );
                                (yyval.keysym) = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            ಇಲ್ಲದಿದ್ದರೆ {
                                /* Any other numeric value */
                                ಆದರೆ ((yyvsp[0].ಅಂಕೆ) <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    (yyval.keysym) = (xkb_keysym_t) (yyvsp[0].ಅಂಕೆ);
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        (yyval.keysym), ಖಾಲಿ, (yyval.keysym), "%#06"PRIx32, "");
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", (yyvsp[0].ಅಂಕೆ), (yyvsp[0].ಅಂಕೆ)
                                    );
                                    (yyval.keysym) = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
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
                                    (yyvsp[0].ಅಂಕೆ), (yyvsp[0].ಅಂಕೆ)
                                );
                            }
                        }
#ಸಾಲು 3683 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 206: /* SignedNumber: "-" Number  */
#ಸಾಲು 1079 "src/xkbcomp/parser.y"
                                        { (yyval.ಅಂಕೆ) = -(yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3689 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 207: /* SignedNumber: Number  */
#ಸಾಲು 1080 "src/xkbcomp/parser.y"
                                        { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3695 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 208: /* Number: "float literal"  */
#ಸಾಲು 1083 "src/xkbcomp/parser.y"
                                      { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3701 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 209: /* Number: "decimal digit"  */
#ಸಾಲು 1084 "src/xkbcomp/parser.y"
                                      { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3707 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 210: /* Number: "integer literal"  */
#ಸಾಲು 1085 "src/xkbcomp/parser.y"
                                      { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3713 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 211: /* Float: "float literal"  */
#ಸಾಲು 1088 "src/xkbcomp/parser.y"
                                { (yyval.ಅಂಕೆ) = 0; }
#ಸಾಲು 3719 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 212: /* Integer: "integer literal"  */
#ಸಾಲು 1091 "src/xkbcomp/parser.y"
                                      { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3725 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 213: /* Integer: "decimal digit"  */
#ಸಾಲು 1092 "src/xkbcomp/parser.y"
                                      { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3731 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 214: /* KeyCode: "integer literal"  */
#ಸಾಲು 1095 "src/xkbcomp/parser.y"
                                      { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3737 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 215: /* KeyCode: "decimal digit"  */
#ಸಾಲು 1096 "src/xkbcomp/parser.y"
                                      { (yyval.ಅಂಕೆ) = (yyvsp[0].ಅಂಕೆ); }
#ಸಾಲು 3743 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 216: /* Ident: "identifier"  */
#ಸಾಲು 1099 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].sval).ಆರಂಭ, (yyvsp[0].sval).len); }
#ಸಾಲು 3749 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 217: /* Ident: "default"  */
#ಸಾಲು 1100 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "default"); }
#ಸಾಲು 3755 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 218: /* String: "string literal"  */
#ಸಾಲು 1103 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].str), ಅಕ್ಷರಸರಣಿ_ಉದ್ದ((yyvsp[0].str))); ಬಿಡುಗಡೆ((yyvsp[0].str)); }
#ಸಾಲು 3761 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 219: /* OptMapName: MapName  */
#ಸಾಲು 1106 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#ಸಾಲು 3767 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 220: /* OptMapName: %empty  */
#ಸಾಲು 1107 "src/xkbcomp/parser.y"
                                { (yyval.str) = ಖಾಲಿ; }
#ಸಾಲು 3773 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;

  ಸ್ಥಿತಿ 221: /* MapName: "string literal"  */
#ಸಾಲು 1110 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#ಸಾಲು 3779 "src/xkbcomp/parser.c"
    ನಿಲ್ಲಿಸು;


#ಸಾಲು 3783 "src/xkbcomp/parser.c"

      ಪೂರ್ವನಿಯೋಜಿತ: ನಿಲ್ಲಿಸು;
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
  YY_ಚಿಹ್ನೆ_PRINT ("-> $$ =", YY_CAST (yysymbol_ಬಗೆ_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ yylhs = yyr1[yyn] - YYNTOKENS;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  ಹೋಗು yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  ಆದರೆ (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *yymsgp = YY_("syntax error");
        ಪೂರ್ಣಾಂಕ yysyntax_error_ಸ್ಥಿತಿ;
        yysyntax_error_ಸ್ಥಿತಿ = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        ಆದರೆ (yysyntax_error_ಸ್ಥಿತಿ == 0)
          yymsgp = yymsg;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (yysyntax_error_ಸ್ಥಿತಿ == -1)
          {
            ಆದರೆ (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (ಅಕ್ಷರ *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            ಆದರೆ (yymsg)
              {
                yysyntax_error_ಸ್ಥಿತಿ
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            ಇಲ್ಲದಿದ್ದರೆ
              {
                yymsg = yymsgbuf;
                yymsg_alloc = ಗಾತ್ರ yymsgbuf;
                yysyntax_error_ಸ್ಥಿತಿ = YYENOMEM;
              }
          }
        yyerror (param, yymsgp);
        ಆದರೆ (yysyntax_error_ಸ್ಥಿತಿ == YYENOMEM)
          YYNOMEM;
      }
    }

  ಆದರೆ (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      ಆದರೆ (yychar <= ಕೊನೆ_ದ_FILE)
        {
          /* Return failure if at end of input.  */
          ಆದರೆ (yychar == ಕೊನೆ_ದ_FILE)
            YYABORT;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, param);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  ಹೋಗು yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  ಆದರೆ (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ರಾಶಿ_PRINT (yyss, yyssp);
  yystate = *yyssp;
  ಹೋಗು yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  ಗಾಗಿ (;;)
    {
      yyn = yypact[yystate];
      ಆದರೆ (!yypact_ಮೌಲ್ಯ_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          ಆದರೆ (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              ಆದರೆ (0 < yyn)
                ನಿಲ್ಲಿಸು;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      ಆದರೆ (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_ಚಿಹ್ನೆ (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_ರಾಶಿ_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_ಕೊನೆ


  /* Shift the error token.  */
  YY_ಚಿಹ್ನೆ_PRINT ("Shifting", YY_ACCESSING_ಚಿಹ್ನೆ (yyn), yyvsp, yylsp);

  yystate = yyn;
  ಹೋಗು yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  ಹೋಗು yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  ಹೋಗು yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  ಹೋಗು yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  ಆದರೆ (yychar != YYEMPTY)
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
  YY_ರಾಶಿ_PRINT (yyss, yyssp);
  ಆಗ (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_ಚಿಹ್ನೆ (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ yyoverflow
  ಆದರೆ (yyss != yyssa)
    YYSTACK_FREE (yyss);
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  ಹಿಂತಿರುಗಿ yyresult;
}

#ಸಾಲು 1113 "src/xkbcomp/parser.y"


/* Parse a specific section */
XkbFile *
parse(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
      ರಚನೆ scanner *scanner, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map)
{
    ಪೂರ್ಣಾಂಕ ret;
    XkbFile *ಮೊದಲ = ಖಾಲಿ;
    ರಚನೆ parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ಖಾಲಿ,
        .config = *config,
        .ಇನ್ನಷ್ಟು_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    ಆಗ ((ret = yyparse(&param)) == 0 && param.ಇನ್ನಷ್ಟು_maps) {
        ಆದರೆ (map) {
            ಆದರೆ (streq_not_null(map, param.rtrn->name))
                ಹಿಂತಿರುಗಿ param.rtrn;
            ಇಲ್ಲದಿದ್ದರೆ
                FreeXkbFile(param.rtrn);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(ಮೊದಲ);
                ಹಿಂತಿರುಗಿ param.rtrn;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಮೊದಲ) {
                ಮೊದಲ = param.rtrn;
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ಖಾಲಿ;
    }

    ಆದರೆ (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(ಮೊದಲ);
        FreeXkbFile(param.rtrn);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    ಆದರೆ (ಮೊದಲ && ಮೊದಲ->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, ಸುರಕ್ಷಿತ_map_name(ಮೊದಲ->name));

    ಹಿಂತಿರುಗಿ ಮೊದಲ;
}

/* Parse the next section */
bool
parse_ಮುಂದಿನ(ರಚನೆ xkb_context *ctx,
           ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
           ರಚನೆ scanner *scanner, XkbFile **xkb_file)
{
    ಪೂರ್ಣಾಂಕ ret;
    ರಚನೆ parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ಖಾಲಿ,
        .config = *config,
        .ಇನ್ನಷ್ಟು_maps = false,
    };

    ಆದರೆ ((ret = yyparse(&param)) == 0 && param.ಇನ್ನಷ್ಟು_maps) {
        *xkb_file = param.rtrn;
        ಹಿಂತಿರುಗಿ true;
    } ಇಲ್ಲದಿದ್ದರೆ {
        FreeXkbFile(param.rtrn);
        *xkb_file = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ (ret == 0);
    }
}
