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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYBISON 30802

/* Bison version string.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYBISON_ਸੰਸਕਰਣ "3.8.2"

/* Skeleton name.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYPURE 1

/* Push parsers.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYPUSH 0

/* Pull parsers.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYPULL 1


/* Substitute the variable and function names.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yyparse         _xkbcommon_parse
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yylex           _xkbcommon_lex
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yyerror         _xkbcommon_error
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yydebug         _xkbcommon_debug
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yynerrs         _xkbcommon_nerrs

/* First part of user prologue.  */
#ਸਤਰ 22 "src/xkbcomp/parser.y"

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/parser-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast-build.h"

ਬਣਤਰ parser_param {
    ਬਣਤਰ xkb_context *ctx;
    ਬਣਤਰ scanner *scanner;
    XkbFile *rtrn;
    ਬਣਤਰ parser_keymap_config config;
    bool ਹੋਰ_maps;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ parser_log_with_ਕੋਡ(param, level, verbosity, log_ਸੁਨੇਹਾ_id, fmt, ...)   \
    scanner_log_with_ਕੋਡ((param)->scanner, level, verbosity, log_ਸੁਨੇਹਾ_id, fmt,\
                          ##__VA_ARGS__)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ਸਥਿਰ ਖਾਲੀ
_xkbcommon_error(ਬਣਤਰ parser_param *param, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", ਸੁਨੇਹਾ);
}

ਸਥਿਰ bool
resolve_keysym(ਬਣਤਰ parser_param *param, ਬਣਤਰ sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    ਜੇ (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
        ਵਾਪਸ true;
    }

    ਜੇ (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_ਕੁੰਜੀ_Void_ਚਿੰਨ੍ਹ;
        ਵਾਪਸ true;
    }

    /* xkb_keysym_from_name needs a C string. */
    ਅੱਖਰ buf[XKB_KEYSYM_NAME_MAX_ਮਾਪ];
    ਜੇ (name.len >= ਆਕਾਰ(buf)) {
        ਵਾਪਸ false;
    }
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, name.ਸ਼ੁਰੂ, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    ਜੇ (sym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        ਵਾਪਸ true;
    }

    ਵਾਪਸ false;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ param_scanner param->scanner

#ਸਤਰ 150 "src/xkbcomp/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  ਨਹੀਂਤਾਂ
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  ਜੇ defined __cplusplus
#   ਜੇ 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   ਨਹੀਂਤਾਂ
#    define YY_NULLPTR 0
#   endif
#  ਨਹੀਂਤਾਂ
#   define YY_NULLPTR ((ਖਾਲੀ*)0)
#  endif
# endif

#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
/* Symbol kind.  */
ਗਿਣਤੀ yysymbol_ਕਿਸਮ_t
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
  YYSYMBOL_ਕੁੰਜੀ = 21,                       /* "key"  */
  YYSYMBOL_ALIAS = 22,                     /* "alias"  */
  YYSYMBOL_ਸਮੂਹ = 23,                     /* "group"  */
  YYSYMBOL_MODIFIER_MAP = 24,              /* "modifier_map"  */
  YYSYMBOL_INDICATOR = 25,                 /* "indicator"  */
  YYSYMBOL_SHAPE = 26,                     /* "shape"  */
  YYSYMBOL_ਕੁੰਜੀਆਂ = 27,                      /* "keys"  */
  YYSYMBOL_ਪੰਕਤੀ = 28,                       /* "row"  */
  YYSYMBOL_SECTION = 29,                   /* "section"  */
  YYSYMBOL_OVERLAY = 30,                   /* "overlay"  */
  YYSYMBOL_ਪਾਠ = 31,                      /* "text"  */
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
  YYSYMBOL_ALPHANUMERIC_ਕੁੰਜੀਆਂ = 61,         /* "alphanumeric_keys"  */
  YYSYMBOL_MODIFIER_ਕੁੰਜੀਆਂ = 62,             /* "modifier_keys"  */
  YYSYMBOL_KEYPAD_ਕੁੰਜੀਆਂ = 63,               /* "keypad_keys"  */
  YYSYMBOL_FUNCTION_ਕੁੰਜੀਆਂ = 64,             /* "function_keys"  */
  YYSYMBOL_ALTERNATE_ਸਮੂਹ = 65,           /* "alternate_group"  */
  YYSYMBOL_DEPRECATED = 66,                /* "deprecated"  */
  YYSYMBOL_YYACCEPT = 67,                  /* $accept  */
  YYSYMBOL_XkbFile = 68,                   /* XkbFile  */
  YYSYMBOL_Xkb_ਸੰਯੁਕਤ_Map = 69,           /* XkbCompositeMap  */
  YYSYMBOL_Xkb_ਸੰਯੁਕਤ_Type = 70,          /* XkbCompositeType  */
  YYSYMBOL_XkbMapConfigList = 71,          /* XkbMapConfigList  */
  YYSYMBOL_XkbMapConfig = 72,              /* XkbMapConfig  */
  YYSYMBOL_FileType = 73,                  /* FileType  */
  YYSYMBOL_OptFlags = 74,                  /* OptFlags  */
  YYSYMBOL_Flags = 75,                     /* Flags  */
  YYSYMBOL_Flag = 76,                      /* Flag  */
  YYSYMBOL_DeclList = 77,                  /* DeclList  */
  YYSYMBOL_Decl = 78,                      /* Decl  */
  YYSYMBOL_VarDecl = 79,                   /* VarDecl  */
  YYSYMBOL_ਕੁੰਜੀ_Name_Decl = 80,               /* KeyNameDecl  */
  YYSYMBOL_ਕੁੰਜੀ_Alias_Decl = 81,              /* KeyAliasDecl  */
  YYSYMBOL_VModDecl = 82,                  /* VModDecl  */
  YYSYMBOL_VModDefList = 83,               /* VModDefList  */
  YYSYMBOL_VModDef = 84,                   /* VModDef  */
  YYSYMBOL_InterpretDecl = 85,             /* InterpretDecl  */
  YYSYMBOL_Interpret_ਮੇਲ = 86,            /* InterpretMatch  */
  YYSYMBOL_VarDeclList = 87,               /* VarDeclList  */
  YYSYMBOL_ਕੁੰਜੀ_Type_Decl = 88,               /* KeyTypeDecl  */
  YYSYMBOL_SymbolsDecl = 89,               /* SymbolsDecl  */
  YYSYMBOL_OptSymbolsBody = 90,            /* OptSymbolsBody  */
  YYSYMBOL_SymbolsBody = 91,               /* SymbolsBody  */
  YYSYMBOL_SymbolsVarDecl = 92,            /* SymbolsVarDecl  */
  YYSYMBOL_Multi_ਕੁੰਜੀ_Sym_Or_Action_List = 93,   /* MultiKeySymOrActionList  */
  YYSYMBOL_No_ਚਿੰਨ੍ਹ_Or_Action_List = 94,      /* NoSymbolOrActionList  */
  YYSYMBOL_ਸਮੂਹ_Compat_Decl = 95,           /* GroupCompatDecl  */
  YYSYMBOL_ModMapDecl = 96,                /* ModMapDecl  */
  YYSYMBOL_ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym_List = 97,           /* KeyOrKeySymList  */
  YYSYMBOL_ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym = 98,               /* KeyOrKeySym  */
  YYSYMBOL_LedMapDecl = 99,                /* LedMapDecl  */
  YYSYMBOL_LedNameDecl = 100,              /* LedNameDecl  */
  YYSYMBOL_UnknownDecl = 101,              /* UnknownDecl  */
  YYSYMBOL_UnknownCompoundStatementDecl = 102, /* UnknownCompoundStatementDecl  */
  YYSYMBOL_ShapeDecl = 103,                /* ShapeDecl  */
  YYSYMBOL_SectionDecl = 104,              /* SectionDecl  */
  YYSYMBOL_SectionBody = 105,              /* SectionBody  */
  YYSYMBOL_Section_Body_ਵਸਤੂ = 106,          /* SectionBodyItem  */
  YYSYMBOL_ਪੰਕਤੀ_Body = 107,                  /* RowBody  */
  YYSYMBOL_ਪੰਕਤੀ_Body_ਵਸਤੂ = 108,              /* RowBodyItem  */
  YYSYMBOL_ਕੁੰਜੀਆਂ_2 = 109,                     /*ਕੁੰਜੀਆਂ*/
  YYSYMBOL_ਕੁੰਜੀ_2 = 110,                      /*ਕੁੰਜੀ*/
  YYSYMBOL_OverlayDecl = 111,              /* OverlayDecl  */
  YYSYMBOL_Overlay_ਕੁੰਜੀ_List = 112,           /* OverlayKeyList  */
  YYSYMBOL_Overlay_ਕੁੰਜੀ = 113,               /* OverlayKey  */
  YYSYMBOL_OutlineList = 114,              /* OutlineList  */
  YYSYMBOL_Outline_ਵਿੱਚ_List = 115,            /* OutlineInList  */
  YYSYMBOL_CoordList = 116,                /* CoordList  */
  YYSYMBOL_Coord = 117,                    /* Coord  */
  YYSYMBOL_DoodadDecl = 118,               /* DoodadDecl  */
  YYSYMBOL_DoodadType = 119,               /* DoodadType  */
  YYSYMBOL_ਖੇਤਰ_Spec = 120,                /* FieldSpec  */
  YYSYMBOL_Element = 121,                  /* Element  */
  YYSYMBOL_Opt_ਮਿਲਾਓ_ਬਹੁਲਕ = 122,             /* OptMergeMode  */
  YYSYMBOL_ਮਿਲਾਓ_ਬਹੁਲਕ = 123,                /* MergeMode  */
  YYSYMBOL_ਵਿਅੰਜਕ_List = 124,                 /* ExprList  */
  YYSYMBOL_ਵਿਅੰਜਕ = 125,                     /*ਵਿਅੰਜਕ*/
  YYSYMBOL_ਪਦ = 126,                     /*ਪਦ*/
  YYSYMBOL_MultiActionList = 127,          /* MultiActionList  */
  YYSYMBOL_ActionList = 128,               /* ActionList  */
  YYSYMBOL_Non_ਖਾਲੀ_ਥਾਵਾਂ_Actions = 129,          /* NonEmptyActions  */
  YYSYMBOL_Actions = 130,                  /* Actions  */
  YYSYMBOL_Action = 131,                   /* Action  */
  YYSYMBOL_Lhs = 132,                      /* Lhs  */
  YYSYMBOL_OptTerminal = 133,              /* OptTerminal  */
  YYSYMBOL_Terminal = 134,                 /* Terminal  */
  YYSYMBOL_Multi_ਕੁੰਜੀ_Sym_List = 135,          /* MultiKeySymList  */
  YYSYMBOL_ਕੁੰਜੀ_Sym_List = 136,               /* KeySymList  */
  YYSYMBOL_Non_ਖਾਲੀ_ਥਾਵਾਂ_ਕੁੰਜੀ_Syms = 137,          /* NonEmptyKeySyms  */
  YYSYMBOL_ਕੁੰਜੀ_Syms = 138,                  /* KeySyms  */
  YYSYMBOL_ਕੁੰਜੀ_Sym = 139,                   /* KeySym  */
  YYSYMBOL_ਕੁੰਜੀ_Sym_Lit = 140,                /* KeySymLit  */
  YYSYMBOL_Signed_ਸੰਖਿਆ = 141,             /* SignedNumber  */
  YYSYMBOL_ਸੰਖਿਆ = 142,                   /*ਸੰਖਿਆ*/
  YYSYMBOL_Float = 143,                    /* Float  */
  YYSYMBOL_Integer = 144,                  /* Integer  */
  YYSYMBOL_ਕੁੰਜੀ_ਕੋਡ = 145,                  /* KeyCode  */
  YYSYMBOL_Ident = 146,                    /* Ident  */
  YYSYMBOL_String = 147,                   /* String  */
  YYSYMBOL_OptMapName = 148,               /* OptMapName  */
  YYSYMBOL_MapName = 149                   /* MapName  */
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ yysymbol_ਕਿਸਮ_t yysymbol_ਕਿਸਮ_t;




#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਛੋਟਾ
# undef ਛੋਟਾ
#ਜੇ_ਅੰਤ

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# ਜੇ defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#ਜੇ_ਅੰਤ

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __INT_LEAST8_MAX__
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __INT_LEAST8_TYPE__ yytype_int8;
#ਜਾਂਜੇ defined YY_STDINT_H
ਕਿਸਮਪਰਿਭਾਸ਼ਾ int_least8_t yytype_int8;
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ yytype_int8;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __INT_LEAST16_MAX__
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __INT_LEAST16_TYPE__ yytype_int16;
#ਜਾਂਜੇ defined YY_STDINT_H
ਕਿਸਮਪਰਿਭਾਸ਼ਾ int_least16_t yytype_int16;
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਛੋਟਾ yytype_int16;
#ਜੇ_ਅੰਤ

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#ਜੇ_ਅੰਤ

#ਜੇ defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __UINT_LEAST8_TYPE__ yytype_uint8;
#ਜਾਂਜੇ (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint_least8_t yytype_uint8;
#ਜਾਂਜੇ !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ yytype_uint8;
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਛੋਟਾ yytype_uint8;
#ਜੇ_ਅੰਤ

#ਜੇ defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __UINT_LEAST16_TYPE__ yytype_uint16;
#ਜਾਂਜੇ (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint_least16_t yytype_uint16;
#ਜਾਂਜੇ !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ yytype_uint16;
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ yytype_uint16;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YYPTRDIFF_T
# ਜੇ defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_ਵੱਧ_ਤੋਂ_ਵੱਧ __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_ਵੱਧ_ਤੋਂ_ਵੱਧ PTRDIFF_MAX
# ਨਹੀਂਤਾਂ
#  define YYPTRDIFF_T ਲੰਮਾ
#  define YYPTRDIFF_ਵੱਧ_ਤੋਂ_ਵੱਧ LONG_MAX
# endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined ਮਾਪ_t
#  define YYSIZE_T ਮਾਪ_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T ਮਾਪ_t
# ਨਹੀਂਤਾਂ
#  define YYSIZE_T ਬਿਨਾਨਿਸ਼ਾਨੀ
# endif
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYSIZE_ਵੱਧ_ਤੋਂ_ਵੱਧ                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_ਵੱਧ_ਤੋਂ_ਵੱਧ < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_ਵੱਧ_ਤੋਂ_ਵੱਧ                         \
            : YY_CAST (YYSIZE_T, -1)))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, ਆਕਾਰ (X))


/* Stored state numbers (used for stacks). */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ yytype_int16 yy_state_t;

/* State numbers in computations.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ yy_state_fast_t;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YY_
# ਜੇ defined YYENABLE_NLS && YYENABLE_NLS
#  ਜੇ ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YY_ATTRIBUTE_ਸ਼ੁੱਧ
# ਜੇ defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_ਸ਼ੁੱਧ __attribute__ ((__pure__))
# ਨਹੀਂਤਾਂ
#  define YY_ATTRIBUTE_ਸ਼ੁੱਧ
# endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YY_ATTRIBUTE_ਅਣਵਰਤਿਆ
# ਜੇ defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_ਅਣਵਰਤਿਆ __attribute__ ((__unused__))
# ਨਹੀਂਤਾਂ
#  define YY_ATTRIBUTE_ਅਣਵਰਤਿਆ
# endif
#ਜੇ_ਅੰਤ

/* Suppress unused-variable warnings by "using" E.  */
#ਜੇ ! defined lint || defined __GNUC__
# define YY_USE(E) ((ਖਾਲੀ) (E))
#ਨਹੀਂਤਾਂ
# define YY_USE(E) /*ਖਾਲੀ_ਥਾਵਾਂ*/
#ਜੇ_ਅੰਤ

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#ਜੇ defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# ਜੇ __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# ਨਹੀਂਤਾਂ
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_ਸਿਰਾ      \
    _Pragma ("GCC diagnostic pop")
#ਨਹੀਂਤਾਂ
# define YY_INITIAL_ਮੁੱਲ(ਮੁੱਲ_2) ਮੁੱਲ_2
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_ਸਿਰਾ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YY_INITIAL_ਮੁੱਲ
# define YY_INITIAL_ਮੁੱਲ(ਮੁੱਲ_2) /* Nothing. */
#ਜੇ_ਅੰਤ

#ਜੇ defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_ਸਿਰਾ            \
    _Pragma ("GCC diagnostic pop")
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_ਸਿਰਾ
#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YY_ASSERT(E) ((ਖਾਲੀ) (0 && (E)))

#ਜੇ 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  ਜੇ YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   ਨਹੀਂਤਾਂ
#    define YYSTACK_ALLOC alloca
#    ਜੇ ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
#  define YYSTACK_FREE(Ptr) ਕਰੋ { /*ਖਾਲੀ_ਥਾਵਾਂ*/; } ਜਦੋਂ (0)
#  ifndef YYSTACK_ALLOC_ਵੱਧ_ਤੋਂ_ਵੱਧ
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_ਵੱਧ_ਤੋਂ_ਵੱਧ 4032 /* reasonable circa 2006 */
#  endif
# ਨਹੀਂਤਾਂ
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_ਵੱਧ_ਤੋਂ_ਵੱਧ
#   define YYSTACK_ALLOC_ਵੱਧ_ਤੋਂ_ਵੱਧ YYSIZE_ਵੱਧ_ਤੋਂ_ਵੱਧ
#  endif
#  ਜੇ (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined ਸਮ੍ਰਿਤੀ_ਦਿਓ) \
             && (defined YYFREE || defined ਮੁਕਤ)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC ਸਮ੍ਰਿਤੀ_ਦਿਓ
#   ਜੇ ! defined ਸਮ੍ਰਿਤੀ_ਦਿਓ && ! defined EXIT_SUCCESS
ਖਾਲੀ *ਸਮ੍ਰਿਤੀ_ਦਿਓ (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ਮੁਕਤ
#   ਜੇ ! defined ਮੁਕਤ && ! defined EXIT_SUCCESS
ਖਾਲੀ ਮੁਕਤ (ਖਾਲੀ *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#ਜੇ_ਅੰਤ /* 1 */

#ਜੇ (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
ਸੰਘ yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_ਵੱਧ_ਤੋਂ_ਵੱਧ (YYSIZEOF (ਸੰਘ yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_ਬਾਈਟਾਂ(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_ਵੱਧ_ਤੋਂ_ਵੱਧ)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(ਢੇਰ_alloc, ਢੇਰ_2)                           \
    ਕਰੋ                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->ਢੇਰ_alloc, ਢੇਰ_2, yysize);                    \
        ਢੇਰ_2 = &yyptr->ਢੇਰ_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*ਢੇਰ_2) + YYSTACK_GAP_ਵੱਧ_ਤੋਂ_ਵੱਧ; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    ਜਦੋਂ (0)

#ਜੇ_ਅੰਤ

#ਜੇ defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  ਜੇ defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, ਗਿਣਤੀ_3) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (ਗਿਣਤੀ_3)) * ਆਕਾਰ (*(Src)))
#  ਨਹੀਂਤਾਂ
#   define YYCOPY(Dst, Src, ਗਿਣਤੀ_3)              \
      ਕਰੋ                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          ਲਈ (yyi = 0; yyi < (ਗਿਣਤੀ_3); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      ਜਦੋਂ (0)
#  endif
# endif
#ਜੇ_ਅੰਤ /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYLAST   945

/* YYNTOKENS -- Number of terminals.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYNNTS  83
/* YYNRULES -- Number of rules.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYNRULES  221
/* YYNSTATES -- Number of states.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYNSTATES  386

/* YYMAXUTOK -- Last valid token kind.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYMAXUTOK   257


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_ਕਿਸਮ_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
ਸਥਿਰ ਅਚਲ yytype_int8 yytranslate[] =
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

#ਜੇ YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
ਸਥਿਰ ਅਚਲ yytype_int16 yyrline[] =
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
#ਜੇ_ਅੰਤ

/** Accessing symbol of state STATE.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YY_ACCESSING_ਚਿੰਨ੍ਹ(State) YY_CAST (yysymbol_ਕਿਸਮ_t, yystos[State])

#ਜੇ 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *yysymbol_name (yysymbol_ਕਿਸਮ_t yysymbol) YY_ATTRIBUTE_ਅਣਵਰਤਿਆ;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
yysymbol_name (yysymbol_ਕਿਸਮ_t yysymbol)
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ yy_sname[] =
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
  ਵਾਪਸ yy_sname[yysymbol];
}
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYPACT_NINF (-225)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yypact_ਮੁੱਲ_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYTABLE_NINF (-217)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yytable_ਮੁੱਲ_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
ਸਥਿਰ ਅਚਲ yytype_int16 yypact[] =
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
ਸਥਿਰ ਅਚਲ yytype_uint8 yydefact[] =
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
ਸਥਿਰ ਅਚਲ yytype_int16 yypgoto[] =
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
ਸਥਿਰ ਅਚਲ yytype_int16 yydefgoto[] =
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
ਸਥਿਰ ਅਚਲ yytype_int16 yytable[] =
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

ਸਥਿਰ ਅਚਲ yytype_int16 yycheck[] =
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
ਸਥਿਰ ਅਚਲ yytype_uint8 yystos[] =
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
ਸਥਿਰ ਅਚਲ yytype_uint8 yyr1[] =
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
ਸਥਿਰ ਅਚਲ yytype_int8 yyr2[] =
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


ਗਿਣਤੀ { YYENOMEM = -2 };

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yyerrok         (yyerrstatus = 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ yyclearin       (yychar = YYEMPTY)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYACCEPT        ਜਾਓ yyacceptlab
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYABORT         ਜਾਓ yyabortlab
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYERROR         ਜਾਓ yyerrorlab
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYNOMEM         ਜਾਓ yyexhaustedlab


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYRECOVERING()  (!!yyerrstatus)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYBACKUP(Token, ਮੁੱਲ_2)                                    \
  ਕਰੋ                                                              \
    ਜੇ (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (ਮੁੱਲ_2);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        ਜਾਓ yybackup;                                            \
      }                                                           \
    ਨਹੀਂਤਾਂ                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  ਜਦੋਂ (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#ਜੇ YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(ਦਲੀਲਾਂ_2)                        \
ਕਰੋ {                                            \
  ਜੇ (yydebug)                                  \
    YYFPRINTF ਦਲੀਲਾਂ_2;                             \
} ਜਦੋਂ (0)




# define YY_ਚਿੰਨ੍ਹ_PRINT(Title, ਕਿਸਮ_2, ਮੁੱਲ_2, Location)                    \
ਕਰੋ {                                                                      \
  ਜੇ (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_ਚਿੰਨ੍ਹ_print (stderr,                                            \
                  ਕਿਸਮ_2, ਮੁੱਲ_2, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} ਜਦੋਂ (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

ਸਥਿਰ ਖਾਲੀ
yy_ਚਿੰਨ੍ਹ_ਮੁੱਲ_print (FILE *yyo,
                       yysymbol_ਕਿਸਮ_t yykind, YYSTYPE ਅਚਲ * ਅਚਲ yyvaluep, ਬਣਤਰ parser_param *param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  ਜੇ (!yyvaluep)
    ਵਾਪਸ;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_ਸਿਰਾ
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

ਸਥਿਰ ਖਾਲੀ
yy_ਚਿੰਨ੍ਹ_print (FILE *yyo,
                 yysymbol_ਕਿਸਮ_t yykind, YYSTYPE ਅਚਲ * ਅਚਲ yyvaluep, ਬਣਤਰ parser_param *param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_ਚਿੰਨ੍ਹ_ਮੁੱਲ_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
yy_ਢੇਰ_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  ਲਈ (; yybottom <= yytop; yybottom++)
    {
      ਪੂਰਨ yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_ਢੇਰ_PRINT(Bottom, ਸਿਖਰ)                            \
ਕਰੋ {                                                            \
  ਜੇ (yydebug)                                                  \
    yy_ਢੇਰ_print ((Bottom), (ਸਿਖਰ));                           \
} ਜਦੋਂ (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 ਪੂਰਨ yyrule, ਬਣਤਰ parser_param *param)
{
  ਪੂਰਨ yylno = yyrline[yyrule];
  ਪੂਰਨ yynrhs = yyr2[yyrule];
  ਪੂਰਨ yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  ਲਈ (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_ਚਿੰਨ੍ਹ_print (stderr,
                       YY_ACCESSING_ਚਿੰਨ੍ਹ (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(ਨਿਯਮ)          \
ਕਰੋ {                                    \
  ਜੇ (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, ਨਿਯਮ, param); \
} ਜਦੋਂ (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
ਪੂਰਨ yydebug;
#ਨਹੀਂਤਾਂ /* !YYDEBUG */
# define YYDPRINTF(ਦਲੀਲਾਂ_2) ((ਖਾਲੀ) 0)
# define YY_ਚਿੰਨ੍ਹ_PRINT(Title, ਕਿਸਮ_2, ਮੁੱਲ_2, Location)
# define YY_ਢੇਰ_PRINT(Bottom, ਸਿਖਰ)
# define YY_REDUCE_PRINT(ਨਿਯਮ)
#ਜੇ_ਅੰਤ /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YYINITDEPTH
# define YYINITDEPTH 200
#ਜੇ_ਅੰਤ

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YYMAXDEPTH
# define YYMAXDEPTH 10000
#ਜੇ_ਅੰਤ


/* Context of a parse error.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  yy_state_t *yyssp;
  yysymbol_ਕਿਸਮ_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
ਸਥਿਰ ਪੂਰਨ
yypcontext_expected_ਟੋਕਨ (ਅਚਲ yypcontext_t *yyctx,
                            yysymbol_ਕਿਸਮ_t yyarg[], ਪੂਰਨ yyargn)
{
  /* Actual size of YYARG. */
  ਪੂਰਨ yycount = 0;
  ਪੂਰਨ yyn = yypact[+*yyctx->yyssp];
  ਜੇ (!yypact_ਮੁੱਲ_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      ਪੂਰਨ yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      ਪੂਰਨ yychecklim = YYLAST - yyn + 1;
      ਪੂਰਨ yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      ਪੂਰਨ yyx;
      ਲਈ (yyx = yyxbegin; yyx < yyxend; ++yyx)
        ਜੇ (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_ਮੁੱਲ_is_error (yytable[yyx + yyn]))
          {
            ਜੇ (!yyarg)
              ++yycount;
            ਨਹੀਂਤਾਂ ਜੇ (yycount == yyargn)
              ਵਾਪਸ 0;
            ਨਹੀਂਤਾਂ
              yyarg[yycount++] = YY_CAST (yysymbol_ਕਿਸਮ_t, yyx);
          }
    }
  ਜੇ (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  ਵਾਪਸ yycount;
}




#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ yystrlen
# ਜੇ defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, ਲੜੀ_ਲੰਬਾਈ (S)))
# ਨਹੀਂਤਾਂ
/* Return the length of YYSTR.  */
ਸਥਿਰ YYPTRDIFF_T
yystrlen (ਅਚਲ ਅੱਖਰ *yystr)
{
  YYPTRDIFF_T yylen;
  ਲਈ (yylen = 0; yystr[yylen]; yylen++)
    ਜਾਰੀ;
  ਵਾਪਸ yylen;
}
# endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ yystpcpy
# ਜੇ defined __GLIBC__ && defined _STRING_H && defined GNU_ਸਰੋਤ
#  define yystpcpy stpcpy
# ਨਹੀਂਤਾਂ
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
ਸਥਿਰ ਅੱਖਰ *
yystpcpy (ਅੱਖਰ *yydest, ਅਚਲ ਅੱਖਰ *yysrc)
{
  ਅੱਖਰ *yyd = yydest;
  ਅਚਲ ਅੱਖਰ *yys = yysrc;

  ਜਦੋਂ ((*yyd++ = *yys++) != '\0')
    ਜਾਰੀ;

  ਵਾਪਸ yyd - 1;
}
# endif
#ਜੇ_ਅੰਤ



ਸਥਿਰ ਪੂਰਨ
yy_syntax_error_arguments (ਅਚਲ yypcontext_t *yyctx,
                           yysymbol_ਕਿਸਮ_t yyarg[], ਪੂਰਨ yyargn)
{
  /* Actual size of YYARG. */
  ਪੂਰਨ yycount = 0;
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
  ਜੇ (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      ਪੂਰਨ yyn;
      ਜੇ (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_ਟੋਕਨ (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      ਜੇ (yyn == YYENOMEM)
        ਵਾਪਸ YYENOMEM;
      ਨਹੀਂਤਾਂ
        yycount += yyn;
    }
  ਵਾਪਸ yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
ਸਥਿਰ ਪੂਰਨ
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, ਅੱਖਰ **yymsg,
                ਅਚਲ yypcontext_t *yyctx)
{
  ਗਿਣਤੀ { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  ਅਚਲ ਅੱਖਰ *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_ਕਿਸਮ_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  ਪੂਰਨ yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  ਜੇ (yycount == YYENOMEM)
    ਵਾਪਸ YYENOMEM;

  ਚੋਣ (yycount)
    {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYCASE_(N, S)                       \
      ਹਾਲਤ N:                               \
        yyformat = S;                       \
        ਰੋਕੋ
    ਮੂਲ: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    ਪੂਰਨ yyi;
    ਲਈ (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        ਜੇ (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_ਵੱਧ_ਤੋਂ_ਵੱਧ)
          yysize = yysize1;
        ਨਹੀਂਤਾਂ
          ਵਾਪਸ YYENOMEM;
      }
  }

  ਜੇ (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      ਜੇ (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_ਵੱਧ_ਤੋਂ_ਵੱਧ))
        *yymsg_alloc = YYSTACK_ALLOC_ਵੱਧ_ਤੋਂ_ਵੱਧ;
      ਵਾਪਸ -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    ਅੱਖਰ *yyp = *yymsg;
    ਪੂਰਨ yyi = 0;
    ਜਦੋਂ ((*yyp = *yyformat) != '\0')
      ਜੇ (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      ਨਹੀਂਤਾਂ
        {
          ++yyp;
          ++yyformat;
        }
  }
  ਵਾਪਸ 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ
yydestruct (ਅਚਲ ਅੱਖਰ *yymsg,
            yysymbol_ਕਿਸਮ_t yykind, YYSTYPE *yyvaluep, ਬਣਤਰ parser_param *param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  ਜੇ (!yymsg)
    yymsg = "Deleting";
  YY_ਚਿੰਨ੍ਹ_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ਚੋਣ (yykind)
    {
    ਹਾਲਤ YYSYMBOL_STRING: /* "string literal"  */
#ਸਤਰ 259 "src/xkbcomp/parser.y"
            { ਮੁਕਤ(((*yyvaluep).str)); }
#ਸਤਰ 1640 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_XkbFile: /* XkbFile  */
#ਸਤਰ 257 "src/xkbcomp/parser.y"
            { ਜੇ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#ਸਤਰ 1646 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Xkb_ਸੰਯੁਕਤ_Map: /* XkbCompositeMap  */
#ਸਤਰ 257 "src/xkbcomp/parser.y"
            { ਜੇ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#ਸਤਰ 1652 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_XkbMapConfigList: /* XkbMapConfigList  */
#ਸਤਰ 258 "src/xkbcomp/parser.y"
            { FreeXkbFile(((*yyvaluep).fileList).ਸਿਰ); }
#ਸਤਰ 1658 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_XkbMapConfig: /* XkbMapConfig  */
#ਸਤਰ 257 "src/xkbcomp/parser.y"
            { ਜੇ (!param->rtrn) FreeXkbFile(((*yyvaluep).file)); }
#ਸਤਰ 1664 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_DeclList: /* DeclList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).anyList).ਸਿਰ); }
#ਸਤਰ 1670 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Decl: /* Decl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).any)); }
#ਸਤਰ 1676 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_VarDecl: /* VarDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#ਸਤਰ 1682 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਕੁੰਜੀ_Name_Decl: /* KeyNameDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਕੁੰਜੀ_ਕੋਡ)); }
#ਸਤਰ 1688 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਕੁੰਜੀ_Alias_Decl: /* KeyAliasDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਕੁੰਜੀ_Alias)); }
#ਸਤਰ 1694 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_VModDecl: /* VModDecl  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).ਸਿਰ); }
#ਸਤਰ 1700 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_VModDefList: /* VModDefList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmodList).ਸਿਰ); }
#ਸਤਰ 1706 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_VModDef: /* VModDef  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).vmod)); }
#ਸਤਰ 1712 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_InterpretDecl: /* InterpretDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#ਸਤਰ 1718 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Interpret_ਮੇਲ: /* InterpretMatch  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).interp)); }
#ਸਤਰ 1724 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_VarDeclList: /* VarDeclList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).ਸਿਰ); }
#ਸਤਰ 1730 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਕੁੰਜੀ_Type_Decl: /* KeyTypeDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਕੁੰਜੀ_Type)); }
#ਸਤਰ 1736 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_SymbolsDecl: /* SymbolsDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).syms)); }
#ਸਤਰ 1742 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_OptSymbolsBody: /* OptSymbolsBody  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).ਸਿਰ); }
#ਸਤਰ 1748 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_SymbolsBody: /* SymbolsBody  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).varList).ਸਿਰ); }
#ਸਤਰ 1754 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_SymbolsVarDecl: /* SymbolsVarDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).var)); }
#ਸਤਰ 1760 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Multi_ਕੁੰਜੀ_Sym_Or_Action_List: /* MultiKeySymOrActionList  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1766 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਸਮੂਹ_Compat_Decl: /* GroupCompatDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਸਮੂਹ_Compat)); }
#ਸਤਰ 1772 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ModMapDecl: /* ModMapDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).mod_ਨਕਾਬ_2)); }
#ਸਤਰ 1778 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym_List: /* KeyOrKeySymList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 1784 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym: /* KeyOrKeySym  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1790 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_LedMapDecl: /* LedMapDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledMap)); }
#ਸਤਰ 1796 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_LedNameDecl: /* LedNameDecl  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ledName)); }
#ਸਤਰ 1802 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_CoordList: /* CoordList  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1808 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Coord: /* Coord  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1814 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਵਿਅੰਜਕ_List: /* ExprList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 1820 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਵਿਅੰਜਕ: /*ਵਿਅੰਜਕ*/
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1826 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਪਦ: /*ਪਦ*/
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1832 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_MultiActionList: /* MultiActionList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 1838 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ActionList: /* ActionList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 1844 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Non_ਖਾਲੀ_ਥਾਵਾਂ_Actions: /* NonEmptyActions  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1850 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Actions: /* Actions  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1856 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Action: /* Action  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1862 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Lhs: /* Lhs  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1868 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_OptTerminal: /* OptTerminal  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1874 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Terminal: /* Terminal  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1880 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Multi_ਕੁੰਜੀ_Sym_List: /* MultiKeySymList  */
#ਸਤਰ 253 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 1886 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਕੁੰਜੀ_Sym_List: /* KeySymList  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1892 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_Non_ਖਾਲੀ_ਥਾਵਾਂ_ਕੁੰਜੀ_Syms: /* NonEmptyKeySyms  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1898 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_ਕੁੰਜੀ_Syms: /* KeySyms  */
#ਸਤਰ 250 "src/xkbcomp/parser.y"
            { FreeStmt((ParseCommon *) ((*yyvaluep).ਵਿਅੰਜਕ)); }
#ਸਤਰ 1904 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_OptMapName: /* OptMapName  */
#ਸਤਰ 259 "src/xkbcomp/parser.y"
            { ਮੁਕਤ(((*yyvaluep).str)); }
#ਸਤਰ 1910 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

    ਹਾਲਤ YYSYMBOL_MapName: /* MapName  */
#ਸਤਰ 259 "src/xkbcomp/parser.y"
            { ਮੁਕਤ(((*yyvaluep).str)); }
#ਸਤਰ 1916 "src/xkbcomp/parser.c"
        ਰੋਕੋ;

      ਮੂਲ:
        ਰੋਕੋ;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_ਸਿਰਾ
}






/*----------.
| yyparse.  |
`----------*/

ਪੂਰਨ
yyparse (ਬਣਤਰ parser_param *param)
{
/* Lookahead token kind.  */
ਪੂਰਨ yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_ਮੁੱਲ (ਸਥਿਰ YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_ਮੁੱਲ (= yyval_default);

    /* Number of syntax errors so far.  */
    ਪੂਰਨ yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    ਪੂਰਨ yyerrstatus = 0;

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

  ਪੂਰਨ yyn;
  /* The return value of yyparse.  */
  ਪੂਰਨ yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_ਕਿਸਮ_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  ਅੱਖਰ yymsgbuf[128];
  ਅੱਖਰ *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = ਆਕਾਰ yymsgbuf;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  ਪੂਰਨ yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  ਜਾਓ yysetstate;


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
  YY_IGNORE_USELESS_CAST_ਸਿਰਾ
  YY_ਢੇਰ_PRINT (yyss, yyssp);

  ਜੇ (yyss + yystacksize - 1 <= yyssp)
#ਜੇ !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#ਨਹੀਂਤਾਂ
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# ਜੇ defined yyoverflow
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
# ਨਹੀਂਤਾਂ /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      ਜੇ (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      ਜੇ (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        ਸੰਘ yyalloc *yyptr =
          YY_CAST (ਸੰਘ yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_ਬਾਈਟਾਂ (yystacksize))));
        ਜੇ (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        ਜੇ (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (ਲੰਮਾ, yystacksize)));
      YY_IGNORE_USELESS_CAST_ਸਿਰਾ

      ਜੇ (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#ਜੇ_ਅੰਤ /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  ਜੇ (yystate == YYFINAL)
    YYACCEPT;

  ਜਾਓ yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  ਜੇ (yypact_ਮੁੱਲ_is_default (yyn))
    ਜਾਓ yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  ਜੇ (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, param_scanner);
    }

  ਜੇ (yychar <= ਸਿਰਾ_ਦਾ_FILE)
    {
      yychar = ਸਿਰਾ_ਦਾ_FILE;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  ਨਹੀਂਤਾਂ ਜੇ (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      ਜਾਓ yyerrlab1;
    }
  ਨਹੀਂਤਾਂ
    {
      yytoken = YYTRANSLATE (yychar);
      YY_ਚਿੰਨ੍ਹ_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  ਜੇ (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    ਜਾਓ yydefault;
  yyn = yytable[yyn];
  ਜੇ (yyn <= 0)
    {
      ਜੇ (yytable_ਮੁੱਲ_is_error (yyn))
        ਜਾਓ yyerrlab;
      yyn = -yyn;
      ਜਾਓ yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  ਜੇ (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_ਚਿੰਨ੍ਹ_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_ਸਿਰਾ

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  ਜਾਓ yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  ਜੇ (yyn == 0)
    ਜਾਓ yyerrlab;
  ਜਾਓ yyreduce;


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
  ਚੋਣ (yyn)
    {
  ਹਾਲਤ 2: /* XkbFile: XkbCompositeMap  */
#ਸਤਰ 276 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->ਹੋਰ_maps = !!param->rtrn; (ਖਾਲੀ) yynerrs; }
#ਸਤਰ 2195 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 3: /* XkbFile: XkbMapConfig  */
#ਸਤਰ 278 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = (yyvsp[0].file); param->ਹੋਰ_maps = !!param->rtrn; YYACCEPT; }
#ਸਤਰ 2201 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 4: /* XkbFile: "end of file"  */
#ਸਤਰ 280 "src/xkbcomp/parser.y"
                        { (yyval.file) = param->rtrn = ਨਿਰਾ; param->ਹੋਰ_maps = false; }
#ਸਤਰ 2207 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 5: /* XkbCompositeMap: OptFlags XkbCompositeType OptMapName "{" XkbMapConfigList "}" ";"  */
#ਸਤਰ 286 "src/xkbcomp/parser.y"
                        { (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (ParseCommon *) (yyvsp[-2].fileList).ਸਿਰ, (yyvsp[-6].mapFlags)); }
#ਸਤਰ 2213 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 6: /* XkbCompositeType: "xkb_keymap"  */
#ਸਤਰ 289 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#ਸਤਰ 2219 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 7: /* XkbCompositeType: "xkb_semantics"  */
#ਸਤਰ 290 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#ਸਤਰ 2225 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 8: /* XkbCompositeType: "xkb_layout"  */
#ਸਤਰ 291 "src/xkbcomp/parser.y"
                                        { (yyval.file_type) = FILE_TYPE_KEYMAP; }
#ਸਤਰ 2231 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 9: /* XkbMapConfigList: XkbMapConfigList XkbMapConfig  */
#ਸਤਰ 298 "src/xkbcomp/parser.y"
                        {
                            ਜੇ ((yyvsp[0].file)) {
                                ਜੇ ((yyvsp[-1].fileList).ਸਿਰ) {
                                    (yyval.fileList).ਸਿਰ = (yyvsp[-1].fileList).ਸਿਰ;
                                    (yyval.fileList).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].file)->common;
                                    (yyval.fileList).ਆਖਰੀ = (yyvsp[0].file);
                                } ਨਹੀਂਤਾਂ {
                                    (yyval.fileList).ਸਿਰ = (yyval.fileList).ਆਖਰੀ = (yyvsp[0].file);
                                }
                            }
                        }
#ਸਤਰ 2247 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 10: /* XkbMapConfigList: %empty  */
#ਸਤਰ 309 "src/xkbcomp/parser.y"
                        { (yyval.fileList).ਸਿਰ = (yyval.fileList).ਆਖਰੀ = ਨਿਰਾ; }
#ਸਤਰ 2253 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 11: /* XkbMapConfig: OptFlags FileType OptMapName "{" DeclList "}" ";"  */
#ਸਤਰ 315 "src/xkbcomp/parser.y"
                        {
                            ਜੇ ((yyvsp[-6].mapFlags) & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            ਸੁਰੱਖਿਅਤ_map_name((yyvsp[-4].str)));
                            }
                            (yyval.file) = XkbFileCreate((yyvsp[-5].file_type), (yyvsp[-4].str), (yyvsp[-2].anyList).ਸਿਰ, (yyvsp[-6].mapFlags));
                        }
#ਸਤਰ 2266 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 12: /* FileType: "xkb_keycodes"  */
#ਸਤਰ 325 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_KEYCODES; }
#ਸਤਰ 2272 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 13: /* FileType: "xkb_types"  */
#ਸਤਰ 326 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_TYPES; }
#ਸਤਰ 2278 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 14: /* FileType: "xkb_compatibility"  */
#ਸਤਰ 327 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_COMPAT; }
#ਸਤਰ 2284 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 15: /* FileType: "xkb_symbols"  */
#ਸਤਰ 328 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_SYMBOLS; }
#ਸਤਰ 2290 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 16: /* FileType: "xkb_geometry"  */
#ਸਤਰ 329 "src/xkbcomp/parser.y"
                                                { (yyval.file_type) = FILE_TYPE_GEOMETRY; }
#ਸਤਰ 2296 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 17: /* OptFlags: Flags  */
#ਸਤਰ 332 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#ਸਤਰ 2302 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 18: /* OptFlags: %empty  */
#ਸਤਰ 333 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = 0; }
#ਸਤਰ 2308 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 19: /* Flags: Flags Flag  */
#ਸਤਰ 336 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = ((yyvsp[-1].mapFlags) | (yyvsp[0].mapFlags)); }
#ਸਤਰ 2314 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 20: /* Flags: Flag  */
#ਸਤਰ 337 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = (yyvsp[0].mapFlags); }
#ਸਤਰ 2320 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 21: /* Flag: "partial"  */
#ਸਤਰ 340 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_PARTIAL; }
#ਸਤਰ 2326 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 22: /* Flag: "default"  */
#ਸਤਰ 341 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEFAULT; }
#ਸਤਰ 2332 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 23: /* Flag: "hidden"  */
#ਸਤਰ 342 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_HIDDEN; }
#ਸਤਰ 2338 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 24: /* Flag: "alphanumeric_keys"  */
#ਸਤਰ 343 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_ALPHANUMERIC; }
#ਸਤਰ 2344 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 25: /* Flag: "modifier_keys"  */
#ਸਤਰ 344 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_MODIFIER; }
#ਸਤਰ 2350 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 26: /* Flag: "keypad_keys"  */
#ਸਤਰ 345 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_KEYPAD; }
#ਸਤਰ 2356 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 27: /* Flag: "function_keys"  */
#ਸਤਰ 346 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_HAS_FN; }
#ਸਤਰ 2362 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 28: /* Flag: "alternate_group"  */
#ਸਤਰ 347 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_ALTGR; }
#ਸਤਰ 2368 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 29: /* Flag: "deprecated"  */
#ਸਤਰ 348 "src/xkbcomp/parser.y"
                                                { (yyval.mapFlags) = MAP_IS_DEPRECATED; }
#ਸਤਰ 2374 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 30: /* Flag: "identifier"  */
#ਸਤਰ 350 "src/xkbcomp/parser.y"
                        {
                            ਅਚਲ bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_ਕੋਡ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (ਬਿਨਾਨਿਸ਼ਾਨੀ)(yyvsp[0].sval).len, (yyvsp[0].sval).ਸ਼ੁਰੂ,
                                (error ? "" : "; ignored")
                            );
                            ਜੇ (error)
                                YYABORT;
                            (yyval.mapFlags) = 0;
                        }
#ਸਤਰ 2393 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 31: /* DeclList: DeclList Decl  */
#ਸਤਰ 367 "src/xkbcomp/parser.y"
                        {
                            ਜੇ ((yyvsp[0].any)) {
                                ਜੇ ((yyvsp[-1].anyList).ਸਿਰ) {
                                    (yyval.anyList).ਸਿਰ = (yyvsp[-1].anyList).ਸਿਰ; (yyvsp[-1].anyList).ਆਖਰੀ->ਅਗਲਾ = (yyvsp[0].any); (yyval.anyList).ਆਖਰੀ = (yyvsp[0].any);
                                } ਨਹੀਂਤਾਂ {
                                    (yyval.anyList).ਸਿਰ = (yyval.anyList).ਆਖਰੀ = (yyvsp[0].any);
                                }
                            }
                        }
#ਸਤਰ 2407 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 32: /* DeclList: DeclList OptMergeMode VModDecl  */
#ਸਤਰ 382 "src/xkbcomp/parser.y"
                        {
                            ਲਈ (VModDef *vmod = (yyvsp[0].vmodList).ਸਿਰ; vmod; vmod = (VModDef *) vmod->common.ਅਗਲਾ)
                                vmod->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            ਜੇ ((yyvsp[-2].anyList).ਸਿਰ) {
                                (yyval.anyList).ਸਿਰ = (yyvsp[-2].anyList).ਸਿਰ; (yyvsp[-2].anyList).ਆਖਰੀ->ਅਗਲਾ = &(yyvsp[0].vmodList).ਸਿਰ->common; (yyval.anyList).ਆਖਰੀ = &(yyvsp[0].vmodList).ਆਖਰੀ->common;
                            } ਨਹੀਂਤਾਂ {
                                (yyval.anyList).ਸਿਰ = &(yyvsp[0].vmodList).ਸਿਰ->common; (yyval.anyList).ਆਖਰੀ = &(yyvsp[0].vmodList).ਆਖਰੀ->common;
                            }
                        }
#ਸਤਰ 2421 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 33: /* DeclList: %empty  */
#ਸਤਰ 391 "src/xkbcomp/parser.y"
                        { (yyval.anyList).ਸਿਰ = (yyval.anyList).ਆਖਰੀ = ਨਿਰਾ; }
#ਸਤਰ 2427 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 34: /* Decl: OptMergeMode VarDecl  */
#ਸਤਰ 395 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].var)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].var);
                        }
#ਸਤਰ 2436 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 35: /* Decl: OptMergeMode InterpretDecl  */
#ਸਤਰ 401 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].interp)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].interp);
                        }
#ਸਤਰ 2445 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 36: /* Decl: OptMergeMode KeyNameDecl  */
#ਸਤਰ 406 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ਕੁੰਜੀ_ਕੋਡ)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ਕੁੰਜੀ_ਕੋਡ);
                        }
#ਸਤਰ 2454 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 37: /* Decl: OptMergeMode KeyAliasDecl  */
#ਸਤਰ 411 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ਕੁੰਜੀ_Alias)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ਕੁੰਜੀ_Alias);
                        }
#ਸਤਰ 2463 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 38: /* Decl: OptMergeMode KeyTypeDecl  */
#ਸਤਰ 416 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ਕੁੰਜੀ_Type)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ਕੁੰਜੀ_Type);
                        }
#ਸਤਰ 2472 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 39: /* Decl: OptMergeMode SymbolsDecl  */
#ਸਤਰ 421 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].syms)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].syms);
                        }
#ਸਤਰ 2481 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 40: /* Decl: OptMergeMode ModMapDecl  */
#ਸਤਰ 426 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].mod_ਨਕਾਬ_2)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].mod_ਨਕਾਬ_2);
                        }
#ਸਤਰ 2490 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 41: /* Decl: OptMergeMode GroupCompatDecl  */
#ਸਤਰ 431 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ਸਮੂਹ_Compat)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ਸਮੂਹ_Compat);
                        }
#ਸਤਰ 2499 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 42: /* Decl: OptMergeMode LedMapDecl  */
#ਸਤਰ 436 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledMap)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledMap);
                        }
#ਸਤਰ 2508 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 43: /* Decl: OptMergeMode LedNameDecl  */
#ਸਤਰ 441 "src/xkbcomp/parser.y"
                        {
                            (yyvsp[0].ledName)->ਮਿਲਾਓ = (yyvsp[-1].ਮਿਲਾਓ);
                            (yyval.any) = (ParseCommon *) (yyvsp[0].ledName);
                        }
#ਸਤਰ 2517 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 44: /* Decl: OptMergeMode ShapeDecl  */
#ਸਤਰ 445 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ਨਿਰਾ; }
#ਸਤਰ 2523 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 45: /* Decl: OptMergeMode SectionDecl  */
#ਸਤਰ 446 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ਨਿਰਾ; }
#ਸਤਰ 2529 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 46: /* Decl: OptMergeMode DoodadDecl  */
#ਸਤਰ 447 "src/xkbcomp/parser.y"
                                                        { (yyval.any) = ਨਿਰਾ; }
#ਸਤਰ 2535 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 47: /* Decl: OptMergeMode UnknownDecl  */
#ਸਤਰ 449 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#ਸਤਰ 2541 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 48: /* Decl: OptMergeMode UnknownCompoundStatementDecl  */
#ਸਤਰ 451 "src/xkbcomp/parser.y"
                            { (yyval.any) = (ParseCommon *) (yyvsp[0].unknown); }
#ਸਤਰ 2547 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 49: /* Decl: MergeMode "string literal"  */
#ਸਤਰ 453 "src/xkbcomp/parser.y"
                        {
                            (yyval.any) = (ParseCommon *) IncludeCreate(param->ctx, (yyvsp[0].str), (yyvsp[-1].ਮਿਲਾਓ));
                            ਮੁਕਤ((yyvsp[0].str));
                        }
#ਸਤਰ 2556 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 50: /* VarDecl: Lhs "=" Expr ";"  */
#ਸਤਰ 460 "src/xkbcomp/parser.y"
                        { (yyval.var) = VarCreate((yyvsp[-3].ਵਿਅੰਜਕ), (yyvsp[-1].ਵਿਅੰਜਕ)); }
#ਸਤਰ 2562 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 51: /* VarDecl: Ident ";"  */
#ਸਤਰ 462 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), true); }
#ਸਤਰ 2568 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 52: /* VarDecl: "!" Ident ";"  */
#ਸਤਰ 464 "src/xkbcomp/parser.y"
                        { (yyval.var) = BoolVarCreate((yyvsp[-1].atom), false); }
#ਸਤਰ 2574 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 53: /* KeyNameDecl: "key name" "=" KeyCode ";"  */
#ਸਤਰ 468 "src/xkbcomp/parser.y"
                        { (yyval.ਕੁੰਜੀ_ਕੋਡ) = KeycodeCreate((yyvsp[-3].atom), (yyvsp[-1].ਅੰਕ)); }
#ਸਤਰ 2580 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 54: /* KeyAliasDecl: "alias" "key name" "=" "key name" ";"  */
#ਸਤਰ 472 "src/xkbcomp/parser.y"
                        { (yyval.ਕੁੰਜੀ_Alias) = ਕੁੰਜੀ_Alias_Create((yyvsp[-3].atom), (yyvsp[-1].atom)); }
#ਸਤਰ 2586 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 55: /* VModDecl: "virtual_modifiers" VModDefList ";"  */
#ਸਤਰ 476 "src/xkbcomp/parser.y"
                        { (yyval.vmodList) = (yyvsp[-1].vmodList); }
#ਸਤਰ 2592 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 56: /* VModDefList: VModDefList "," VModDef  */
#ਸਤਰ 480 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).ਸਿਰ = (yyvsp[-2].vmodList).ਸਿਰ; (yyval.vmodList).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].vmod)->common; (yyval.vmodList).ਆਖਰੀ = (yyvsp[0].vmod); }
#ਸਤਰ 2598 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 57: /* VModDefList: VModDef  */
#ਸਤਰ 482 "src/xkbcomp/parser.y"
                        { (yyval.vmodList).ਸਿਰ = (yyval.vmodList).ਆਖਰੀ = (yyvsp[0].vmod); }
#ਸਤਰ 2604 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 58: /* VModDef: Ident  */
#ਸਤਰ 486 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[0].atom), ਨਿਰਾ); }
#ਸਤਰ 2610 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 59: /* VModDef: Ident "=" Expr  */
#ਸਤਰ 488 "src/xkbcomp/parser.y"
                        { (yyval.vmod) = VModCreate((yyvsp[-2].atom), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 2616 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 60: /* InterpretDecl: "interpret" InterpretMatch "{" VarDeclList "}" ";"  */
#ਸਤਰ 494 "src/xkbcomp/parser.y"
                        { (yyvsp[-4].interp)->def = (yyvsp[-2].varList).ਸਿਰ; (yyval.interp) = (yyvsp[-4].interp); }
#ਸਤਰ 2622 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 61: /* InterpretMatch: KeySym "+" Expr  */
#ਸਤਰ 498 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[-2].keysym), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 2628 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 62: /* InterpretMatch: KeySym  */
#ਸਤਰ 500 "src/xkbcomp/parser.y"
                        { (yyval.interp) = InterpCreate((yyvsp[0].keysym), ਨਿਰਾ); }
#ਸਤਰ 2634 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 63: /* VarDeclList: VarDeclList VarDecl  */
#ਸਤਰ 504 "src/xkbcomp/parser.y"
                        {
                            ਜੇ ((yyvsp[0].var)) {
                                ਜੇ ((yyvsp[-1].varList).ਸਿਰ) {
                                    (yyval.varList).ਸਿਰ = (yyvsp[-1].varList).ਸਿਰ;
                                    (yyval.varList).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].var)->common;
                                    (yyval.varList).ਆਖਰੀ = (yyvsp[0].var);
                                } ਨਹੀਂਤਾਂ {
                                    (yyval.varList).ਸਿਰ = (yyval.varList).ਆਖਰੀ = (yyvsp[0].var);
                                }
                            }
                        }
#ਸਤਰ 2650 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 64: /* VarDeclList: %empty  */
#ਸਤਰ 515 "src/xkbcomp/parser.y"
                        { (yyval.varList).ਸਿਰ = (yyval.varList).ਆਖਰੀ = ਨਿਰਾ; }
#ਸਤਰ 2656 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 65: /* KeyTypeDecl: "type" String "{" VarDeclList "}" ";"  */
#ਸਤਰ 521 "src/xkbcomp/parser.y"
                        { (yyval.ਕੁੰਜੀ_Type) = ਕੁੰਜੀ_Type_Create((yyvsp[-4].atom), (yyvsp[-2].varList).ਸਿਰ); }
#ਸਤਰ 2662 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 66: /* SymbolsDecl: "key" "key name" "{" OptSymbolsBody "}" ";"  */
#ਸਤਰ 527 "src/xkbcomp/parser.y"
                        { (yyval.syms) = SymbolsCreate((yyvsp[-4].atom), (yyvsp[-2].varList).ਸਿਰ); }
#ਸਤਰ 2668 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 67: /* OptSymbolsBody: SymbolsBody  */
#ਸਤਰ 530 "src/xkbcomp/parser.y"
                                    { (yyval.varList) = (yyvsp[0].varList); }
#ਸਤਰ 2674 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 68: /* OptSymbolsBody: %empty  */
#ਸਤਰ 531 "src/xkbcomp/parser.y"
                                    { (yyval.varList).ਸਿਰ = (yyval.varList).ਆਖਰੀ = ਨਿਰਾ; }
#ਸਤਰ 2680 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 69: /* SymbolsBody: SymbolsBody "," SymbolsVarDecl  */
#ਸਤਰ 535 "src/xkbcomp/parser.y"
                        { (yyval.varList).ਸਿਰ = (yyvsp[-2].varList).ਸਿਰ; (yyval.varList).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].var)->common; (yyval.varList).ਆਖਰੀ = (yyvsp[0].var); }
#ਸਤਰ 2686 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 70: /* SymbolsBody: SymbolsVarDecl  */
#ਸਤਰ 537 "src/xkbcomp/parser.y"
                        { (yyval.varList).ਸਿਰ = (yyval.varList).ਆਖਰੀ = (yyvsp[0].var); }
#ਸਤਰ 2692 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 71: /* SymbolsVarDecl: Lhs "=" Expr  */
#ਸਤਰ 540 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate((yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 2698 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 72: /* SymbolsVarDecl: Lhs "=" MultiKeySymOrActionList  */
#ਸਤਰ 541 "src/xkbcomp/parser.y"
                                                           { (yyval.var) = VarCreate((yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 2704 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 73: /* SymbolsVarDecl: Ident  */
#ਸਤਰ 542 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), true); }
#ਸਤਰ 2710 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 74: /* SymbolsVarDecl: "!" Ident  */
#ਸਤਰ 543 "src/xkbcomp/parser.y"
                                                { (yyval.var) = BoolVarCreate((yyvsp[0].atom), false); }
#ਸਤਰ 2716 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 75: /* SymbolsVarDecl: MultiKeySymOrActionList  */
#ਸਤਰ 544 "src/xkbcomp/parser.y"
                                                { (yyval.var) = VarCreate(ਨਿਰਾ, (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 2722 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 76: /* MultiKeySymOrActionList: "[" MultiKeySymList "]"  */
#ਸਤਰ 560 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ; }
#ਸਤਰ 2728 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 77: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiKeySymList "]"  */
#ਸਤਰ 562 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoSymbol */
                            ਬਣਤਰ {ਵਿਅੰਜਕ_Def *ਸਿਰ; ਵਿਅੰਜਕ_Def *ਆਖਰੀ;} list = {
                                .ਸਿਰ = (yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ, .ਆਖਰੀ = (yyvsp[-1].ਵਿਅੰਜਕ_List).ਆਖਰੀ
                            };
                            ਲਈ (uint32_t k = 0; k < (yyvsp[-3].no_ਚਿੰਨ੍ਹ_Or_Action_List); k++) {
                                ਵਿਅੰਜਕ_Def* ਅਚਲ syms =
                                    ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
                                ਜੇ (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.ਅਗਲਾ = &list.ਸਿਰ->common;
                                list.ਸਿਰ = syms;
                            }
                            (yyval.ਵਿਅੰਜਕ) = list.ਸਿਰ;
                        }
#ਸਤਰ 2750 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 78: /* MultiKeySymOrActionList: "[" MultiActionList "]"  */
#ਸਤਰ 580 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ; }
#ਸਤਰ 2756 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 79: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "," MultiActionList "]"  */
#ਸਤਰ 582 "src/xkbcomp/parser.y"
                        {
                            /* Prepend n times NoAction() */
                            ਬਣਤਰ {ਵਿਅੰਜਕ_Def *ਸਿਰ; ਵਿਅੰਜਕ_Def *ਆਖਰੀ;} list = {
                                .ਸਿਰ = (yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ, .ਆਖਰੀ = (yyvsp[-1].ਵਿਅੰਜਕ_List).ਆਖਰੀ
                            };
                            ਲਈ (uint32_t k = 0; k < (yyvsp[-3].no_ਚਿੰਨ੍ਹ_Or_Action_List); k++) {
                                ਵਿਅੰਜਕ_Def* ਅਚਲ acts = ਵਿਅੰਜਕ_Create_Action_List(ਨਿਰਾ);
                                ਜੇ (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.ਅਗਲਾ = &list.ਸਿਰ->common;
                                list.ਸਿਰ = acts;
                            }
                            (yyval.ਵਿਅੰਜਕ) = list.ਸਿਰ;
                        }
#ਸਤਰ 2777 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 80: /* MultiKeySymOrActionList: "[" NoSymbolOrActionList "]"  */
#ਸਤਰ 604 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_List(); }
#ਸਤਰ 2783 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 81: /* NoSymbolOrActionList: NoSymbolOrActionList "," "{" "}"  */
#ਸਤਰ 610 "src/xkbcomp/parser.y"
                        { (yyval.no_ਚਿੰਨ੍ਹ_Or_Action_List) = (yyvsp[-3].no_ਚਿੰਨ੍ਹ_Or_Action_List) + 1; }
#ਸਤਰ 2789 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 82: /* NoSymbolOrActionList: "{" "}"  */
#ਸਤਰ 612 "src/xkbcomp/parser.y"
                        { (yyval.no_ਚਿੰਨ੍ਹ_Or_Action_List) = 1; }
#ਸਤਰ 2795 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 83: /* NoSymbolOrActionList: %empty  */
#ਸਤਰ 613 "src/xkbcomp/parser.y"
                        { (yyval.no_ਚਿੰਨ੍ਹ_Or_Action_List) = 0; }
#ਸਤਰ 2801 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 84: /* GroupCompatDecl: "group" Integer "=" Expr ";"  */
#ਸਤਰ 617 "src/xkbcomp/parser.y"
                        { (yyval.ਸਮੂਹ_Compat) = ਸਮੂਹ_Compat_Create((yyvsp[-3].ਅੰਕ), (yyvsp[-1].ਵਿਅੰਜਕ)); }
#ਸਤਰ 2807 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 85: /* ModMapDecl: "modifier_map" Expr "{" KeyOrKeySymList "}" ";"  */
#ਸਤਰ 621 "src/xkbcomp/parser.y"
                        {
                            ਜੇ (param->config.format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1 &&
                                (yyvsp[-4].ਵਿਅੰਜਕ)->common.type != STMT_ਵਿਅੰਜਕ_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ਨਕਾਬ,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) (yyvsp[-4].ਵਿਅੰਜਕ));
                                    FreeStmt((ParseCommon *) (yyvsp[-2].ਵਿਅੰਜਕ_List).ਸਿਰ);
                                    YYERROR;
                            }
                            (yyval.mod_ਨਕਾਬ_2) = ModMapCreate((yyvsp[-4].ਵਿਅੰਜਕ), (yyvsp[-2].ਵਿਅੰਜਕ_List).ਸਿਰ);
                        }
#ਸਤਰ 2826 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 86: /* KeyOrKeySymList: KeyOrKeySymList "," KeyOrKeySym  */
#ਸਤਰ 638 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyvsp[-2].ਵਿਅੰਜਕ_List).ਸਿਰ; (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].ਵਿਅੰਜਕ)->common; (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 2832 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 87: /* KeyOrKeySymList: KeyOrKeySym  */
#ਸਤਰ 640 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 2838 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 88: /* KeyOrKeySym: "key name"  */
#ਸਤਰ 644 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Name((yyvsp[0].atom)); }
#ਸਤਰ 2844 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 89: /* KeyOrKeySym: KeySym  */
#ਸਤਰ 646 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym((yyvsp[0].keysym)); }
#ਸਤਰ 2850 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 90: /* LedMapDecl: "indicator" String "{" VarDeclList "}" ";"  */
#ਸਤਰ 650 "src/xkbcomp/parser.y"
                        { (yyval.ledMap) = LedMapCreate((yyvsp[-4].atom), (yyvsp[-2].varList).ਸਿਰ); }
#ਸਤਰ 2856 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 91: /* LedNameDecl: "indicator" Integer "=" Expr ";"  */
#ਸਤਰ 654 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].ਅੰਕ), (yyvsp[-1].ਵਿਅੰਜਕ), false); }
#ਸਤਰ 2862 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 92: /* LedNameDecl: "virtual" "indicator" Integer "=" Expr ";"  */
#ਸਤਰ 656 "src/xkbcomp/parser.y"
                        { (yyval.ledName) = LedNameCreate((yyvsp[-3].ਅੰਕ), (yyvsp[-1].ਵਿਅੰਜਕ), true); }
#ਸਤਰ 2868 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 93: /* UnknownDecl: "identifier" Terminal "=" Expr ";"  */
#ਸਤਰ 660 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-3].ਵਿਅੰਜਕ));
                            FreeStmt((ParseCommon *) (yyvsp[-1].ਵਿਅੰਜਕ));
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, (yyvsp[-4].sval));
                        }
#ਸਤਰ 2878 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 94: /* UnknownCompoundStatementDecl: "identifier" OptTerminal "{" VarDeclList "}" ";"  */
#ਸਤਰ 669 "src/xkbcomp/parser.y"
                        {
                            FreeStmt((ParseCommon *) (yyvsp[-4].ਵਿਅੰਜਕ));
                            FreeStmt((ParseCommon *) (yyvsp[-2].varList).ਸਿਰ);
                            (yyval.unknown) = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, (yyvsp[-5].sval));
                        }
#ਸਤਰ 2888 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 95: /* ShapeDecl: "shape" String "{" OutlineList "}" ";"  */
#ਸਤਰ 677 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2894 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 96: /* ShapeDecl: "shape" String "{" CoordList "}" ";"  */
#ਸਤਰ 679 "src/xkbcomp/parser.y"
                        { (ਖਾਲੀ) (yyvsp[-2].ਵਿਅੰਜਕ); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2900 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 97: /* SectionDecl: "section" String "{" SectionBody "}" ";"  */
#ਸਤਰ 683 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2906 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 98: /* SectionBody: SectionBody SectionBodyItem  */
#ਸਤਰ 686 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ਨਿਰਾ;}
#ਸਤਰ 2912 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 99: /* SectionBody: SectionBodyItem  */
#ਸਤਰ 687 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2918 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 100: /* SectionBodyItem: "row" "{" RowBody "}" ";"  */
#ਸਤਰ 691 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2924 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 101: /* SectionBodyItem: VarDecl  */
#ਸਤਰ 693 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2930 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 102: /* SectionBodyItem: DoodadDecl  */
#ਸਤਰ 695 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2936 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 103: /* SectionBodyItem: LedMapDecl  */
#ਸਤਰ 697 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ledMap)); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2942 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 104: /* SectionBodyItem: OverlayDecl  */
#ਸਤਰ 699 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2948 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 105: /* RowBody: RowBody RowBodyItem  */
#ਸਤਰ 702 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ਨਿਰਾ;}
#ਸਤਰ 2954 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 106: /* RowBody: RowBodyItem  */
#ਸਤਰ 703 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2960 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 107: /* RowBodyItem: "keys" "{" Keys "}" ";"  */
#ਸਤਰ 706 "src/xkbcomp/parser.y"
                                                     { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2966 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 108: /* RowBodyItem: VarDecl  */
#ਸਤਰ 708 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].var)); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2972 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 109: /* Keys: Keys "," Key  */
#ਸਤਰ 711 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2978 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 110: /* Keys: Key  */
#ਸਤਰ 712 "src/xkbcomp/parser.y"
                                                { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2984 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 111: /* Key: "key name"  */
#ਸਤਰ 716 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2990 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 112: /* Key: "{" ExprList "}"  */
#ਸਤਰ 718 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 2996 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 113: /* OverlayDecl: "overlay" String "{" OverlayKeyList "}" ";"  */
#ਸਤਰ 722 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3002 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 114: /* OverlayKeyList: OverlayKeyList "," OverlayKey  */
#ਸਤਰ 725 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3008 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 115: /* OverlayKeyList: OverlayKey  */
#ਸਤਰ 726 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3014 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 116: /* OverlayKey: "key name" "=" "key name"  */
#ਸਤਰ 729 "src/xkbcomp/parser.y"
                                                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3020 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 117: /* OutlineList: OutlineList "," OutlineInList  */
#ਸਤਰ 733 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ;}
#ਸਤਰ 3026 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 118: /* OutlineList: OutlineInList  */
#ਸਤਰ 735 "src/xkbcomp/parser.y"
                        { (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3032 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 119: /* OutlineInList: "{" CoordList "}"  */
#ਸਤਰ 739 "src/xkbcomp/parser.y"
                        { (ਖਾਲੀ) (yyvsp[-1].ਵਿਅੰਜਕ); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3038 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 120: /* OutlineInList: Ident "=" "{" CoordList "}"  */
#ਸਤਰ 741 "src/xkbcomp/parser.y"
                        { (ਖਾਲੀ) (yyvsp[-1].ਵਿਅੰਜਕ); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3044 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 121: /* OutlineInList: Ident "=" Expr  */
#ਸਤਰ 743 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[0].ਵਿਅੰਜਕ)); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3050 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 122: /* CoordList: CoordList "," Coord  */
#ਸਤਰ 747 "src/xkbcomp/parser.y"
                        { (ਖਾਲੀ) (yyvsp[-2].ਵਿਅੰਜਕ); (ਖਾਲੀ) (yyvsp[0].ਵਿਅੰਜਕ); (yyval.ਵਿਅੰਜਕ) = ਨਿਰਾ; }
#ਸਤਰ 3056 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 123: /* CoordList: Coord  */
#ਸਤਰ 749 "src/xkbcomp/parser.y"
                        { (ਖਾਲੀ) (yyvsp[0].ਵਿਅੰਜਕ); (yyval.ਵਿਅੰਜਕ) = ਨਿਰਾ; }
#ਸਤਰ 3062 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 124: /* Coord: "[" SignedNumber "," SignedNumber "]"  */
#ਸਤਰ 753 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਨਿਰਾ; }
#ਸਤਰ 3068 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 125: /* DoodadDecl: DoodadType String "{" VarDeclList "}" ";"  */
#ਸਤਰ 757 "src/xkbcomp/parser.y"
                        { FreeStmt((ParseCommon *) (yyvsp[-2].varList).ਸਿਰ); (yyval.geom) = ਨਿਰਾ; }
#ਸਤਰ 3074 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 126: /* DoodadType: "text"  */
#ਸਤਰ 760 "src/xkbcomp/parser.y"
                                { (yyval.ਅੰਕ) = 0; }
#ਸਤਰ 3080 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 127: /* DoodadType: "outline"  */
#ਸਤਰ 761 "src/xkbcomp/parser.y"
                                { (yyval.ਅੰਕ) = 0; }
#ਸਤਰ 3086 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 128: /* DoodadType: "solid"  */
#ਸਤਰ 762 "src/xkbcomp/parser.y"
                                { (yyval.ਅੰਕ) = 0; }
#ਸਤਰ 3092 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 129: /* DoodadType: "logo"  */
#ਸਤਰ 763 "src/xkbcomp/parser.y"
                                { (yyval.ਅੰਕ) = 0; }
#ਸਤਰ 3098 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 130: /* FieldSpec: Ident  */
#ਸਤਰ 766 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#ਸਤਰ 3104 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 131: /* FieldSpec: Element  */
#ਸਤਰ 767 "src/xkbcomp/parser.y"
                                { (yyval.atom) = (yyvsp[0].atom); }
#ਸਤਰ 3110 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 132: /* Element: "action"  */
#ਸਤਰ 771 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "action"); }
#ਸਤਰ 3116 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 133: /* Element: "interpret"  */
#ਸਤਰ 773 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "interpret"); }
#ਸਤਰ 3122 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 134: /* Element: "type"  */
#ਸਤਰ 775 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "type"); }
#ਸਤਰ 3128 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 135: /* Element: "key"  */
#ਸਤਰ 777 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "key"); }
#ਸਤਰ 3134 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 136: /* Element: "group"  */
#ਸਤਰ 779 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "group"); }
#ਸਤਰ 3140 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 137: /* Element: "modifier_map"  */
#ਸਤਰ 781 "src/xkbcomp/parser.y"
                        {(yyval.atom) = xkb_atom_intern_literal(param->ctx, "modifier_map");}
#ਸਤਰ 3146 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 138: /* Element: "indicator"  */
#ਸਤਰ 783 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "indicator"); }
#ਸਤਰ 3152 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 139: /* Element: "shape"  */
#ਸਤਰ 785 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "shape"); }
#ਸਤਰ 3158 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 140: /* Element: "row"  */
#ਸਤਰ 787 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "row"); }
#ਸਤਰ 3164 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 141: /* Element: "section"  */
#ਸਤਰ 789 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "section"); }
#ਸਤਰ 3170 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 142: /* Element: "text"  */
#ਸਤਰ 791 "src/xkbcomp/parser.y"
                        { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "text"); }
#ਸਤਰ 3176 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 143: /* OptMergeMode: MergeMode  */
#ਸਤਰ 794 "src/xkbcomp/parser.y"
                                        { (yyval.ਮਿਲਾਓ) = (yyvsp[0].ਮਿਲਾਓ); }
#ਸਤਰ 3182 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 144: /* OptMergeMode: %empty  */
#ਸਤਰ 795 "src/xkbcomp/parser.y"
                                        { (yyval.ਮਿਲਾਓ) = ਮਿਲਾਓ_DEFAULT; }
#ਸਤਰ 3188 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 145: /* MergeMode: "include"  */
#ਸਤਰ 798 "src/xkbcomp/parser.y"
                                        { (yyval.ਮਿਲਾਓ) = ਮਿਲਾਓ_DEFAULT; }
#ਸਤਰ 3194 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 146: /* MergeMode: "augment"  */
#ਸਤਰ 799 "src/xkbcomp/parser.y"
                                        { (yyval.ਮਿਲਾਓ) = ਮਿਲਾਓ_AUGMENT; }
#ਸਤਰ 3200 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 147: /* MergeMode: "override"  */
#ਸਤਰ 800 "src/xkbcomp/parser.y"
                                        { (yyval.ਮਿਲਾਓ) = ਮਿਲਾਓ_OVERRIDE; }
#ਸਤਰ 3206 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 148: /* MergeMode: "replace"  */
#ਸਤਰ 801 "src/xkbcomp/parser.y"
                                        { (yyval.ਮਿਲਾਓ) = ਮਿਲਾਓ_REPLACE; }
#ਸਤਰ 3212 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 149: /* MergeMode: "alternate"  */
#ਸਤਰ 803 "src/xkbcomp/parser.y"
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    (yyval.ਮਿਲਾਓ) = ਮਿਲਾਓ_DEFAULT;
                }
#ਸਤਰ 3226 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 150: /* ExprList: ExprList "," Expr  */
#ਸਤਰ 815 "src/xkbcomp/parser.y"
                        {
                            ਜੇ ((yyvsp[0].ਵਿਅੰਜਕ)) {
                                ਜੇ ((yyvsp[-2].ਵਿਅੰਜਕ_List).ਸਿਰ) {
                                    (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyvsp[-2].ਵਿਅੰਜਕ_List).ਸਿਰ;
                                    (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].ਵਿਅੰਜਕ)->common;
                                    (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ);
                                } ਨਹੀਂਤਾਂ {
                                    (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ);
                                }
                            }
                        }
#ਸਤਰ 3242 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 151: /* ExprList: Expr  */
#ਸਤਰ 827 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3248 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 152: /* ExprList: %empty  */
#ਸਤਰ 828 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = ਨਿਰਾ; }
#ਸਤਰ 3254 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 153: /* Expr: Expr "/" Expr  */
#ਸਤਰ 832 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_DIVIDE, (yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3260 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 154: /* Expr: Expr "+" Expr  */
#ਸਤਰ 834 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_ਜੋੜੋ, (yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3266 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 155: /* Expr: Expr "-" Expr  */
#ਸਤਰ 836 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_ਘਟਾਓ, (yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3272 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 156: /* Expr: Expr "*" Expr  */
#ਸਤਰ 838 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_MULTIPLY, (yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3278 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 157: /* Expr: Lhs "=" Expr  */
#ਸਤਰ 840 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ, (yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3284 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 158: /* Expr: Term  */
#ਸਤਰ 842 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3290 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 159: /* Term: "-" Term  */
#ਸਤਰ 846 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_NEGATE, (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3296 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 160: /* Term: "+" Term  */
#ਸਤਰ 848 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_UNARY_PLUS, (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3302 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 161: /* Term: "!" Term  */
#ਸਤਰ 850 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_NOT, (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3308 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 162: /* Term: "~" Term  */
#ਸਤਰ 852 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_INVERT, (yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3314 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 163: /* Term: Lhs  */
#ਸਤਰ 854 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3320 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 164: /* Term: FieldSpec "(" ExprList ")"  */
#ਸਤਰ 856 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Action((yyvsp[-3].atom), (yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 3326 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 165: /* Term: Actions  */
#ਸਤਰ 858 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3332 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 166: /* Term: Terminal  */
#ਸਤਰ 860 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3338 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 167: /* Term: "(" Expr ")"  */
#ਸਤਰ 862 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[-1].ਵਿਅੰਜਕ); }
#ਸਤਰ 3344 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 168: /* MultiActionList: MultiActionList "," Action  */
#ਸਤਰ 866 "src/xkbcomp/parser.y"
                        {
                            ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create_Action_List((yyvsp[0].ਵਿਅੰਜਕ));
                            (yyval.ਵਿਅੰਜਕ_List) = (yyvsp[-2].ਵਿਅੰਜਕ_List);
                            (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ->common.ਅਗਲਾ = &ਵਿਅੰਜਕ->common; (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = ਵਿਅੰਜਕ;
                        }
#ਸਤਰ 3354 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 169: /* MultiActionList: MultiActionList "," Actions  */
#ਸਤਰ 872 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List) = (yyvsp[-2].ਵਿਅੰਜਕ_List); (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].ਵਿਅੰਜਕ)->common; (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3360 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 170: /* MultiActionList: Action  */
#ਸਤਰ 874 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = ਵਿਅੰਜਕ_Create_Action_List((yyvsp[0].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3366 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 171: /* MultiActionList: NonEmptyActions  */
#ਸਤਰ 876 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3372 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 172: /* ActionList: ActionList "," Action  */
#ਸਤਰ 880 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List) = (yyvsp[-2].ਵਿਅੰਜਕ_List); (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].ਵਿਅੰਜਕ)->common; (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3378 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 173: /* ActionList: Action  */
#ਸਤਰ 882 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3384 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 174: /* NonEmptyActions: "{" ActionList "}"  */
#ਸਤਰ 886 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Action_List((yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 3390 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 175: /* Actions: NonEmptyActions  */
#ਸਤਰ 890 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3396 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 176: /* Actions: "{" "}"  */
#ਸਤਰ 892 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Action_List(ਨਿਰਾ); }
#ਸਤਰ 3402 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 177: /* Action: FieldSpec "(" ExprList ")"  */
#ਸਤਰ 896 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Action((yyvsp[-3].atom), (yyvsp[-1].ਵਿਅੰਜਕ_List).ਸਿਰ); }
#ਸਤਰ 3408 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 178: /* Lhs: FieldSpec  */
#ਸਤਰ 900 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Ident((yyvsp[0].atom)); }
#ਸਤਰ 3414 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 179: /* Lhs: FieldSpec "." FieldSpec  */
#ਸਤਰ 902 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਖੇਤਰ_Ref((yyvsp[-2].atom), (yyvsp[0].atom)); }
#ਸਤਰ 3420 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 180: /* Lhs: FieldSpec "[" Expr "]"  */
#ਸਤਰ 904 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Array_Ref(XKB_ATOM_NONE, (yyvsp[-3].atom), (yyvsp[-1].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3426 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 181: /* Lhs: FieldSpec "." FieldSpec "[" Expr "]"  */
#ਸਤਰ 906 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Array_Ref((yyvsp[-5].atom), (yyvsp[-3].atom), (yyvsp[-1].ਵਿਅੰਜਕ)); }
#ਸਤਰ 3432 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 182: /* OptTerminal: Terminal  */
#ਸਤਰ 910 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3438 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 183: /* OptTerminal: %empty  */
#ਸਤਰ 911 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਨਿਰਾ; }
#ਸਤਰ 3444 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 184: /* Terminal: String  */
#ਸਤਰ 915 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_String((yyvsp[0].atom)); }
#ਸਤਰ 3450 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 185: /* Terminal: Integer  */
#ਸਤਰ 917 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Integer((yyvsp[0].ਅੰਕ)); }
#ਸਤਰ 3456 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 186: /* Terminal: Float  */
#ਸਤਰ 919 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_Float(/* Discard $1 */); }
#ਸਤਰ 3462 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 187: /* Terminal: "key name"  */
#ਸਤਰ 921 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Name((yyvsp[0].atom)); }
#ਸਤਰ 3468 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 188: /* MultiKeySymList: MultiKeySymList "," KeySymLit  */
#ਸਤਰ 925 "src/xkbcomp/parser.y"
                        {
                            ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List((yyvsp[0].keysym));
                            (yyval.ਵਿਅੰਜਕ_List) = (yyvsp[-2].ਵਿਅੰਜਕ_List);
                            (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ->common.ਅਗਲਾ = &ਵਿਅੰਜਕ->common; (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = ਵਿਅੰਜਕ;
                        }
#ਸਤਰ 3478 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 189: /* MultiKeySymList: MultiKeySymList "," KeySyms  */
#ਸਤਰ 931 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List) = (yyvsp[-2].ਵਿਅੰਜਕ_List); (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ->common.ਅਗਲਾ = &(yyvsp[0].ਵਿਅੰਜਕ)->common; (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3484 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 190: /* MultiKeySymList: KeySymLit  */
#ਸਤਰ 933 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List((yyvsp[0].keysym)); }
#ਸਤਰ 3490 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 191: /* MultiKeySymList: NonEmptyKeySyms  */
#ਸਤਰ 935 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ_List).ਸਿਰ = (yyval.ਵਿਅੰਜਕ_List).ਆਖਰੀ = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3496 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 192: /* KeySymList: KeySymList "," KeySymLit  */
#ਸਤਰ 939 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Append_ਕੁੰਜੀ_Sym_List((yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].keysym)); }
#ਸਤਰ 3502 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 193: /* KeySymList: KeySymList "," "string literal"  */
#ਸਤਰ 941 "src/xkbcomp/parser.y"
                        {
                            (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(param->scanner, (yyvsp[-2].ਵਿਅੰਜਕ), (yyvsp[0].str));
                            ਮੁਕਤ((yyvsp[0].str));
                            ਜੇ (!(yyval.ਵਿਅੰਜਕ))
                                YYERROR;
                        }
#ਸਤਰ 3513 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 194: /* KeySymList: KeySymLit  */
#ਸਤਰ 948 "src/xkbcomp/parser.y"
                        {
                            (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List((yyvsp[0].keysym));
                            ਜੇ (!(yyval.ਵਿਅੰਜਕ))
                                YYERROR;
                        }
#ਸਤਰ 3523 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 195: /* KeySymList: "string literal"  */
#ਸਤਰ 954 "src/xkbcomp/parser.y"
                        {
                            (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
                            ਜੇ (!(yyval.ਵਿਅੰਜਕ))
                                YYERROR;
                            (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(param->scanner, (yyval.ਵਿਅੰਜਕ), (yyvsp[0].str));
                            ਮੁਕਤ((yyvsp[0].str));
                            ਜੇ (!(yyval.ਵਿਅੰਜਕ))
                                YYERROR;
                        }
#ਸਤਰ 3537 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 196: /* NonEmptyKeySyms: "{" KeySymList "}"  */
#ਸਤਰ 966 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[-1].ਵਿਅੰਜਕ); }
#ਸਤਰ 3543 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 197: /* NonEmptyKeySyms: "string literal"  */
#ਸਤਰ 968 "src/xkbcomp/parser.y"
                        {
                            (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
                            ਜੇ (!(yyval.ਵਿਅੰਜਕ))
                                YYERROR;
                            (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(param->scanner, (yyval.ਵਿਅੰਜਕ), (yyvsp[0].str));
                            ਮੁਕਤ((yyvsp[0].str));
                            ਜੇ (!(yyval.ਵਿਅੰਜਕ))
                                YYERROR;
                        }
#ਸਤਰ 3557 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 198: /* KeySyms: NonEmptyKeySyms  */
#ਸਤਰ 980 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = (yyvsp[0].ਵਿਅੰਜਕ); }
#ਸਤਰ 3563 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 199: /* KeySyms: "{" "}"  */
#ਸਤਰ 982 "src/xkbcomp/parser.y"
                        { (yyval.ਵਿਅੰਜਕ) = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ); }
#ਸਤਰ 3569 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 200: /* KeySym: KeySymLit  */
#ਸਤਰ 986 "src/xkbcomp/parser.y"
                        { (yyval.keysym) = (yyvsp[0].keysym); }
#ਸਤਰ 3575 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 201: /* KeySym: "string literal"  */
#ਸਤਰ 988 "src/xkbcomp/parser.y"
                        {
                            (yyval.keysym) = KeysymParseString(param->scanner, (yyvsp[0].str));
                            ਮੁਕਤ((yyvsp[0].str));
                            ਜੇ ((yyval.keysym) == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ)
                                YYERROR;
                        }
#ਸਤਰ 3586 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 202: /* KeySymLit: "identifier"  */
#ਸਤਰ 997 "src/xkbcomp/parser.y"
                        {
                            ਜੇ (!resolve_keysym(param, (yyvsp[0].sval), &(yyval.keysym))) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (yyvsp[0].sval).len, (yyvsp[0].sval).ਸ਼ੁਰੂ
                                );
                                (yyval.keysym) = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
                            }
                        }
#ਸਤਰ 3602 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 203: /* KeySymLit: "section"  */
#ਸਤਰ 1009 "src/xkbcomp/parser.y"
                                { (yyval.keysym) = XKB_ਕੁੰਜੀ_section; }
#ਸਤਰ 3608 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 204: /* KeySymLit: "decimal digit"  */
#ਸਤਰ 1011 "src/xkbcomp/parser.y"
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            (yyval.keysym) = XKB_ਕੁੰਜੀ_0 + (xkb_keysym_t) (yyvsp[0].ਅੰਕ);
                        }
#ਸਤਰ 3621 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 205: /* KeySymLit: "integer literal"  */
#ਸਤਰ 1020 "src/xkbcomp/parser.y"
                        {
                            ਜੇ ((yyvsp[0].ਅੰਕ) < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -(yyvsp[0].ਅੰਕ), (yyvsp[0].ਅੰਕ)
                                );
                                (yyval.keysym) = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            ਨਹੀਂਤਾਂ {
                                /* Any other numeric value */
                                ਜੇ ((yyvsp[0].ਅੰਕ) <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    (yyval.keysym) = (xkb_keysym_t) (yyvsp[0].ਅੰਕ);
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        (yyval.keysym), ਨਿਰਾ, (yyval.keysym), "%#06"PRIx32, "");
                                } ਨਹੀਂਤਾਂ {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", (yyvsp[0].ਅੰਕ), (yyvsp[0].ਅੰਕ)
                                    );
                                    (yyval.keysym) = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
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
                                    (yyvsp[0].ਅੰਕ), (yyvsp[0].ਅੰਕ)
                                );
                            }
                        }
#ਸਤਰ 3683 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 206: /* SignedNumber: "-" Number  */
#ਸਤਰ 1079 "src/xkbcomp/parser.y"
                                        { (yyval.ਅੰਕ) = -(yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3689 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 207: /* SignedNumber: Number  */
#ਸਤਰ 1080 "src/xkbcomp/parser.y"
                                        { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3695 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 208: /* Number: "float literal"  */
#ਸਤਰ 1083 "src/xkbcomp/parser.y"
                                      { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3701 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 209: /* Number: "decimal digit"  */
#ਸਤਰ 1084 "src/xkbcomp/parser.y"
                                      { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3707 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 210: /* Number: "integer literal"  */
#ਸਤਰ 1085 "src/xkbcomp/parser.y"
                                      { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3713 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 211: /* Float: "float literal"  */
#ਸਤਰ 1088 "src/xkbcomp/parser.y"
                                { (yyval.ਅੰਕ) = 0; }
#ਸਤਰ 3719 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 212: /* Integer: "integer literal"  */
#ਸਤਰ 1091 "src/xkbcomp/parser.y"
                                      { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3725 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 213: /* Integer: "decimal digit"  */
#ਸਤਰ 1092 "src/xkbcomp/parser.y"
                                      { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3731 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 214: /* KeyCode: "integer literal"  */
#ਸਤਰ 1095 "src/xkbcomp/parser.y"
                                      { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3737 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 215: /* KeyCode: "decimal digit"  */
#ਸਤਰ 1096 "src/xkbcomp/parser.y"
                                      { (yyval.ਅੰਕ) = (yyvsp[0].ਅੰਕ); }
#ਸਤਰ 3743 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 216: /* Ident: "identifier"  */
#ਸਤਰ 1099 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].sval).ਸ਼ੁਰੂ, (yyvsp[0].sval).len); }
#ਸਤਰ 3749 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 217: /* Ident: "default"  */
#ਸਤਰ 1100 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern_literal(param->ctx, "default"); }
#ਸਤਰ 3755 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 218: /* String: "string literal"  */
#ਸਤਰ 1103 "src/xkbcomp/parser.y"
                                { (yyval.atom) = xkb_atom_intern(param->ctx, (yyvsp[0].str), ਲੜੀ_ਲੰਬਾਈ((yyvsp[0].str))); ਮੁਕਤ((yyvsp[0].str)); }
#ਸਤਰ 3761 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 219: /* OptMapName: MapName  */
#ਸਤਰ 1106 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#ਸਤਰ 3767 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 220: /* OptMapName: %empty  */
#ਸਤਰ 1107 "src/xkbcomp/parser.y"
                                { (yyval.str) = ਨਿਰਾ; }
#ਸਤਰ 3773 "src/xkbcomp/parser.c"
    ਰੋਕੋ;

  ਹਾਲਤ 221: /* MapName: "string literal"  */
#ਸਤਰ 1110 "src/xkbcomp/parser.y"
                                { (yyval.str) = (yyvsp[0].str); }
#ਸਤਰ 3779 "src/xkbcomp/parser.c"
    ਰੋਕੋ;


#ਸਤਰ 3783 "src/xkbcomp/parser.c"

      ਮੂਲ: ਰੋਕੋ;
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
  YY_ਚਿੰਨ੍ਹ_PRINT ("-> $$ =", YY_CAST (yysymbol_ਕਿਸਮ_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    ਅਚਲ ਪੂਰਨ yylhs = yyr1[yyn] - YYNTOKENS;
    ਅਚਲ ਪੂਰਨ yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  ਜਾਓ yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  ਜੇ (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        ਅੱਖਰ ਅਚਲ *yymsgp = YY_("syntax error");
        ਪੂਰਨ yysyntax_error_ਸਥਿਤੀ;
        yysyntax_error_ਸਥਿਤੀ = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        ਜੇ (yysyntax_error_ਸਥਿਤੀ == 0)
          yymsgp = yymsg;
        ਨਹੀਂਤਾਂ ਜੇ (yysyntax_error_ਸਥਿਤੀ == -1)
          {
            ਜੇ (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (ਅੱਖਰ *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            ਜੇ (yymsg)
              {
                yysyntax_error_ਸਥਿਤੀ
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            ਨਹੀਂਤਾਂ
              {
                yymsg = yymsgbuf;
                yymsg_alloc = ਆਕਾਰ yymsgbuf;
                yysyntax_error_ਸਥਿਤੀ = YYENOMEM;
              }
          }
        yyerror (param, yymsgp);
        ਜੇ (yysyntax_error_ਸਥਿਤੀ == YYENOMEM)
          YYNOMEM;
      }
    }

  ਜੇ (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      ਜੇ (yychar <= ਸਿਰਾ_ਦਾ_FILE)
        {
          /* Return failure if at end of input.  */
          ਜੇ (yychar == ਸਿਰਾ_ਦਾ_FILE)
            YYABORT;
        }
      ਨਹੀਂਤਾਂ
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, param);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  ਜਾਓ yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  ਜੇ (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_ਢੇਰ_PRINT (yyss, yyssp);
  yystate = *yyssp;
  ਜਾਓ yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  ਲਈ (;;)
    {
      yyn = yypact[yystate];
      ਜੇ (!yypact_ਮੁੱਲ_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          ਜੇ (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              ਜੇ (0 < yyn)
                ਰੋਕੋ;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      ਜੇ (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_ਚਿੰਨ੍ਹ (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_ਢੇਰ_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_ਸਿਰਾ


  /* Shift the error token.  */
  YY_ਚਿੰਨ੍ਹ_PRINT ("Shifting", YY_ACCESSING_ਚਿੰਨ੍ਹ (yyn), yyvsp, yylsp);

  yystate = yyn;
  ਜਾਓ yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  ਜਾਓ yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  ਜਾਓ yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  ਜਾਓ yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  ਜੇ (yychar != YYEMPTY)
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
  YY_ਢੇਰ_PRINT (yyss, yyssp);
  ਜਦੋਂ (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_ਚਿੰਨ੍ਹ (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ yyoverflow
  ਜੇ (yyss != yyssa)
    YYSTACK_FREE (yyss);
#ਜੇ_ਅੰਤ
  ਜੇ (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  ਵਾਪਸ yyresult;
}

#ਸਤਰ 1113 "src/xkbcomp/parser.y"


/* Parse a specific section */
XkbFile *
parse(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
      ਬਣਤਰ scanner *scanner, ਅਚਲ ਅੱਖਰ *map)
{
    ਪੂਰਨ ret;
    XkbFile *ਪਹਿਲਾ = ਨਿਰਾ;
    ਬਣਤਰ parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ਨਿਰਾ,
        .config = *config,
        .ਹੋਰ_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    ਜਦੋਂ ((ret = yyparse(&param)) == 0 && param.ਹੋਰ_maps) {
        ਜੇ (map) {
            ਜੇ (streq_not_null(map, param.rtrn->name))
                ਵਾਪਸ param.rtrn;
            ਨਹੀਂਤਾਂ
                FreeXkbFile(param.rtrn);
        }
        ਨਹੀਂਤਾਂ {
            ਜੇ (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(ਪਹਿਲਾ);
                ਵਾਪਸ param.rtrn;
            }
            ਨਹੀਂਤਾਂ ਜੇ (!ਪਹਿਲਾ) {
                ਪਹਿਲਾ = param.rtrn;
            }
            ਨਹੀਂਤਾਂ {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ਨਿਰਾ;
    }

    ਜੇ (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(ਪਹਿਲਾ);
        FreeXkbFile(param.rtrn);
        ਵਾਪਸ ਨਿਰਾ;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    ਜੇ (ਪਹਿਲਾ && ਪਹਿਲਾ->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, ਸੁਰੱਖਿਅਤ_map_name(ਪਹਿਲਾ->name));

    ਵਾਪਸ ਪਹਿਲਾ;
}

/* Parse the next section */
bool
parse_ਅਗਲਾ(ਬਣਤਰ xkb_context *ctx,
           ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
           ਬਣਤਰ scanner *scanner, XkbFile **xkb_file)
{
    ਪੂਰਨ ret;
    ਬਣਤਰ parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ਨਿਰਾ,
        .config = *config,
        .ਹੋਰ_maps = false,
    };

    ਜੇ ((ret = yyparse(&param)) == 0 && param.ਹੋਰ_maps) {
        *xkb_file = param.rtrn;
        ਵਾਪਸ true;
    } ਨਹੀਂਤਾਂ {
        FreeXkbFile(param.rtrn);
        *xkb_file = ਨਿਰਾ;
        ਵਾਪਸ (ret == 0);
    }
}
