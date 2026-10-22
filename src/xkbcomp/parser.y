/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

/*
 * The parser should work with reasonably recent versions of bison.
 *
 * byacc support was dropped, because it does not support the following features:
 * - `%define parse.error detailed`
 */

/* The following goes in the header */
%code requires {
#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/xkbcomp-priv.h"
}

%{
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
    ಅಕ್ಷರ buf[XKB_KEYSYM_NAME_MAX_SIZE];
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
%}

%define api.ಶುದ್ಧ
%lex-param      { ರಚನೆ scanner *param_scanner }
%parse-param    { ರಚನೆ parser_param *param }

%define parse.error detailed

%token
        ಕೊನೆ_ದ_FILE     0 "end of file"
        ERROR_TOK       255 "invalid token"
        XKB_KEYMAP      1 "xkb_keymap"
        XKB_KEYCODES    2 "xkb_keycodes"
        XKB_TYPES       3 "xkb_types"
        XKB_SYMBOLS     4 "xkb_symbols"
        XKB_COMPATMAP   5 "xkb_compatibility"
        XKB_GEOMETRY    6 "xkb_geometry"
        XKB_SEMANTICS   7 "xkb_semantics"
        XKB_LAYOUT      8 "xkb_layout"
        INCLUDE         10 "include"
        OVERRIDE        11 "override"
        AUGMENT         12 "augment"
        REPLACE         13 "replace"
        ALTERNATE       14 "alternate"
        VIRTUAL_MODS    20 "virtual_modifiers"
        TYPE            21 "type"
        INTERPRET       22 "interpret"
        ACTION_TOK      23 "action"
        ಕೀಲಿ_3             24 "key"
        ALIAS           25 "alias"
        ಗುಂಪು_2           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        ಕೀಲಿಗಳು_2            30 "keys"
        ಅಡ್ಡಸಾಲು             31 "row"
        SECTION         32 "section"
        OVERLAY         33 "overlay"
        ಪಠ್ಯ            34 "text"
        OUTLINE         35 "outline"
        SOLID           36 "solid"
        LOGO            37 "logo"
        VIRTUAL         38 "virtual"
        EQUALS          40 "="
        PLUS            41 "+"
        MINUS           42 "-"
        DIVIDE          43 "/"
        TIMES           44 "*"
        OBRACE          45 "{"
        CBRACE          46 "}"
        OPAREN          47 "("
        CPAREN          48 ")"
        OBRACKET        49 "["
        CBRACKET        50 "]"
        DOT             51 "."
        COMMA           52 ","
        SEMI            53 ";"
        EXCLAM          54 "!"
        INVERT          55 "~"
        STRING          60 "string literal"
        DECIMAL_DIGIT   61 "decimal digit"
        INTEGER         62 "integer literal"
        FLOAT           63 "float literal"
        IDENT           64 "identifier"
        KEYNAME         65 "key name"
        PARTIAL         70 "partial"
        DEFAULT         71 "default"
        HIDDEN          72 "hidden"
        ALPHANUMERIC_ಕೀಲಿಗಳು       73 "alphanumeric_keys"
        MODIFIER_ಕೀಲಿಗಳು           74 "modifier_keys"
        KEYPAD_ಕೀಲಿಗಳು             75 "keypad_keys"
        FUNCTION_ಕೀಲಿಗಳು           76 "function_keys"
        ALTERNATE_ಗುಂಪು         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%ಬಲ  EQUALS
%ಎಡ   PLUS MINUS
%ಎಡ   TIMES DIVIDE
%ಎಡ   EXCLAM INVERT
%ಎಡ   OPAREN

%start  XkbFile

%ಒಕ್ಕೂಟ  {
        int64_t          ಅಂಕೆ;
        ಎಣಿಕೆ xkb_file_type file_type;
        ಅಕ್ಷರ            *str;
        ರಚನೆ sval     sval;
        xkb_atom_t      atom;
        ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
        ಎಣಿಕೆ xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        ರಚನೆ { ParseCommon *ತಲೆ; ParseCommon *ಕೊನೆಯ; } anyList;
        uint32_t        no_ಚಿಹ್ನೆ_Or_Action_List;
        ಅಭಿವ್ಯಕ್ತಿ_Def         *ಅಭಿವ್ಯಕ್ತಿ;
        ರಚನೆ { ಅಭಿವ್ಯಕ್ತಿ_Def *ತಲೆ; ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೊನೆಯ; } ಅಭಿವ್ಯಕ್ತಿ_List;
        VarDef          *var;
        ರಚನೆ { VarDef *ತಲೆ; VarDef *ಕೊನೆಯ; } varList;
        VModDef         *vmod;
        ರಚನೆ { VModDef *ತಲೆ; VModDef *ಕೊನೆಯ; } vmodList;
        InterpDef       *interp;
        ಕೀಲಿ_Type_Def      *ಕೀಲಿ_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_ಮುಖವಾಡ_2;
        ಗುಂಪು_Compat_Def  *ಗುಂಪು_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *ಕೀಲಿ_ಸಂಕೇತ;
        ಕೀಲಿ_Alias_Def     *ಕೀಲಿ_Alias;
        UnknownStatement *unknown;
        ಶೂನ್ಯ            *geom;
        XkbFile         *file;
        ರಚನೆ { XkbFile *ತಲೆ; XkbFile *ಕೊನೆಯ; } fileList;
}

%ಪ್ರಕಾರ <ಅಂಕೆ>     DECIMAL_DIGIT INTEGER FLOAT
%ಪ್ರಕಾರ <str>     STRING
%ಪ್ರಕಾರ <sval>    IDENT
%ಪ್ರಕಾರ <atom>    KEYNAME
%ಪ್ರಕಾರ <ಅಂಕೆ>     ಕೀಲಿ_ಸಂಕೇತ_2 ಸಂಖ್ಯೆ Integer Float Signed_ಸಂಖ್ಯೆ DoodadType
%ಪ್ರಕಾರ <ವಿಲೀನ>   ವಿಲೀನ_ಬಹುಲಕ_2 Opt_ವಿಲೀನ_ಬಹುಲಕ
%ಪ್ರಕಾರ <file_type> Xkb_ಸಂಯುಕ್ತ_Type FileType
%ಪ್ರಕಾರ <mapFlags> Flag Flags OptFlags
%ಪ್ರಕಾರ <str>     MapName OptMapName
%ಪ್ರಕಾರ <atom>    ಕ್ಷೇತ್ರ_Spec Ident Element String
%ಪ್ರಕಾರ <keysym>  ಕೀಲಿ_Sym_2 ಕೀಲಿ_Sym_Lit
%ಪ್ರಕಾರ <no_ಚಿಹ್ನೆ_Or_Action_List> No_ಚಿಹ್ನೆ_Or_Action_List
%ಪ್ರಕಾರ <any>     Decl
%ಪ್ರಕಾರ <anyList> DeclList
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ>    ಅಭಿವ್ಯಕ್ತಿ_2 ಪದ Lhs Terminal OptTerminal Coord CoordList
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ>    Multi_ಕೀಲಿ_Sym_Or_Action_List Non_ಖಾಲಿಗಳು_Actions Actions Action
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ>    ಕೀಲಿ_Syms Non_ಖಾಲಿಗಳು_ಕೀಲಿ_Syms ಕೀಲಿ_Sym_List ಕೀಲಿ_Or_ಕೀಲಿ_Sym
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ_List> ಅಭಿವ್ಯಕ್ತಿ_List_2 ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ_List> ActionList MultiActionList Multi_ಕೀಲಿ_Sym_List
%ಪ್ರಕಾರ <var>     VarDecl SymbolsVarDecl
%ಪ್ರಕಾರ <varList> VarDeclList SymbolsBody OptSymbolsBody
%ಪ್ರಕಾರ <vmod>    VModDef
%ಪ್ರಕಾರ <vmodList> VModDefList VModDecl
%ಪ್ರಕಾರ <interp>  InterpretDecl Interpret_ಹೊಂದಿಕೆ
%ಪ್ರಕಾರ <ಕೀಲಿ_Type> ಕೀಲಿ_Type_Decl
%ಪ್ರಕಾರ <syms>    SymbolsDecl
%ಪ್ರಕಾರ <mod_ಮುಖವಾಡ_2> ModMapDecl
%ಪ್ರಕಾರ <ಗುಂಪು_Compat> ಗುಂಪು_Compat_Decl
%ಪ್ರಕಾರ <ledMap>  LedMapDecl
%ಪ್ರಕಾರ <ledName> LedNameDecl
%ಪ್ರಕಾರ <ಕೀಲಿ_ಸಂಕೇತ> ಕೀಲಿ_Name_Decl
%ಪ್ರಕಾರ <ಕೀಲಿ_Alias> ಕೀಲಿ_Alias_Decl
%ಪ್ರಕಾರ <geom>    ShapeDecl SectionDecl SectionBody Section_Body_ಅಂಶ ಅಡ್ಡಸಾಲು_Body ಅಡ್ಡಸಾಲು_Body_ಅಂಶ
%ಪ್ರಕಾರ <geom>    ಕೀಲಿಗಳು_3 ಕೀಲಿ_4 OverlayDecl Overlay_ಕೀಲಿ_List Overlay_ಕೀಲಿ OutlineList Outline_ಒಳ_List
%ಪ್ರಕಾರ <geom>    DoodadDecl
%ಪ್ರಕಾರ <unknown> UnknownDecl UnknownCompoundStatementDecl
%ಪ್ರಕಾರ <file>    XkbFile XkbMapConfig
%ಪ್ರಕಾರ <fileList> XkbMapConfigList
%ಪ್ರಕಾರ <file>    Xkb_ಸಂಯುಕ್ತ_Map

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <ಅಭಿವ್ಯಕ್ತಿ> <var> <vmod> <interp> <ಕೀಲಿ_Type> <syms> <mod_ಮುಖವಾಡ_2> <ಗುಂಪು_Compat>
    <ledMap> <ledName> <ಕೀಲಿ_ಸಂಕೇತ> <ಕೀಲಿ_Alias>
%destructor { FreeStmt((ParseCommon *) $$.ತಲೆ); }
    <anyList> <ಅಭಿವ್ಯಕ್ತಿ_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { ಆದರೆ (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.ತಲೆ); } <fileList>
%destructor { ಬಿಡುಗಡೆ($$); } <str>

%%

/*
 * An actual file may contain more than one map. However, if we do things
 * in the normal yacc way, i.e. aggregate all of the maps into a list and
 * let the caller find the map it wants, we end up scanning and parsing a
 * lot of unneeded maps (in the end we always just need one).
 * Instead of doing that, we make yyparse return one map at a time, and
 * then call it repeatedly until we find the map we need. Once we find it,
 * we don't need to parse everything that follows in the file.
 * This does mean that if we e.g. always use the first map, the file may
 * contain complete garbage after that. But it's worth it.
 */

XkbFile         :       Xkb_ಸಂಯುಕ್ತ_Map
                        { $$ = param->rtrn = $1; param->ಇನ್ನಷ್ಟು_maps = !!param->rtrn; (ಶೂನ್ಯ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->ಇನ್ನಷ್ಟು_maps = !!param->rtrn; YYACCEPT; }
                |       ಕೊನೆ_ದ_FILE
                        { $$ = param->rtrn = ಖಾಲಿ; param->ಇನ್ನಷ್ಟು_maps = false; }
                ;

Xkb_ಸಂಯುಕ್ತ_Map :       OptFlags Xkb_ಸಂಯುಕ್ತ_Type OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.ತಲೆ, $1); }
                ;

Xkb_ಸಂಯುಕ್ತ_Type:       XKB_KEYMAP      { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_SEMANTICS   { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_LAYOUT      { $$ = FILE_TYPE_KEYMAP; }
                ;

/*
 * NOTE: Any component is optional
 */
XkbMapConfigList :      XkbMapConfigList XkbMapConfig
                        {
                            ಆದರೆ ($2) {
                                ಆದರೆ ($1.ತಲೆ) {
                                    $$.ತಲೆ = $1.ತಲೆ;
                                    $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$2->common;
                                    $$.ಕೊನೆಯ = $2;
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    $$.ತಲೆ = $$.ಕೊನೆಯ = $2;
                                }
                            }
                        }
                |       { $$.ತಲೆ = $$.ಕೊನೆಯ = ಖಾಲಿ; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            ಆದರೆ ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            ಸುರಕ್ಷಿತ_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.ತಲೆ, $1);
                        }
                ;

FileType        :       XKB_KEYCODES            { $$ = FILE_TYPE_KEYCODES; }
                |       XKB_TYPES               { $$ = FILE_TYPE_TYPES; }
                |       XKB_COMPATMAP           { $$ = FILE_TYPE_COMPAT; }
                |       XKB_SYMBOLS             { $$ = FILE_TYPE_SYMBOLS; }
                |       XKB_GEOMETRY            { $$ = FILE_TYPE_GEOMETRY; }
                ;

OptFlags        :       Flags                   { $$ = $1; }
                |                               { $$ = 0; }
                ;

Flags           :       Flags Flag              { $$ = ($1 | $2); }
                |       Flag                    { $$ = $1; }
                ;

Flag            :       PARTIAL                 { $$ = MAP_IS_PARTIAL; }
                |       DEFAULT                 { $$ = MAP_IS_DEFAULT; }
                |       HIDDEN                  { $$ = MAP_IS_HIDDEN; }
                |       ALPHANUMERIC_ಕೀಲಿಗಳು       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_ಕೀಲಿಗಳು           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_ಕೀಲಿಗಳು             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_ಕೀಲಿಗಳು           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_ಗುಂಪು         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ಸ್ಥಿರಾಂಕ bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_ಸಂಕೇತ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (ಚಿಹ್ನೆರಹಿತ)$1.len, $1.ಆರಂಭ,
                                (error ? "" : "; ignored")
                            );
                            ಆದರೆ (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            ಆದರೆ ($2) {
                                ಆದರೆ ($1.ತಲೆ) {
                                    $$.ತಲೆ = $1.ತಲೆ; $1.ಕೊನೆಯ->ಮುಂದಿನ = $2; $$.ಕೊನೆಯ = $2;
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    $$.ತಲೆ = $$.ಕೊನೆಯ = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList Opt_ವಿಲೀನ_ಬಹುಲಕ VModDecl
                        {
                            ಗಾಗಿ (VModDef *vmod = $3.ತಲೆ; vmod; vmod = (VModDef *) vmod->common.ಮುಂದಿನ)
                                vmod->ವಿಲೀನ = $2;
                            ಆದರೆ ($1.ತಲೆ) {
                                $$.ತಲೆ = $1.ತಲೆ; $1.ಕೊನೆಯ->ಮುಂದಿನ = &$3.ತಲೆ->common; $$.ಕೊನೆಯ = &$3.ಕೊನೆಯ->common;
                            } ಇಲ್ಲದಿದ್ದರೆ {
                                $$.ತಲೆ = &$3.ತಲೆ->common; $$.ಕೊನೆಯ = &$3.ಕೊನೆಯ->common;
                            }
                        }
                |       { $$.ತಲೆ = $$.ಕೊನೆಯ = ಖಾಲಿ; }
                ;

Decl            :       Opt_ವಿಲೀನ_ಬಹುಲಕ VarDecl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ InterpretDecl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ ಕೀಲಿ_Name_Decl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ ಕೀಲಿ_Alias_Decl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ ಕೀಲಿ_Type_Decl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ SymbolsDecl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ ModMapDecl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ ಗುಂಪು_Compat_Decl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ LedMapDecl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ LedNameDecl
                        {
                            $2->ವಿಲೀನ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ ShapeDecl          { $$ = ಖಾಲಿ; }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ SectionDecl        { $$ = ಖಾಲಿ; }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ DoodadDecl         { $$ = ಖಾಲಿ; }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       Opt_ವಿಲೀನ_ಬಹುಲಕ UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       ವಿಲೀನ_ಬಹುಲಕ_2 STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            ಬಿಡುಗಡೆ($2);
                        }
                ;

VarDecl         :       Lhs EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

ಕೀಲಿ_Name_Decl     :       KEYNAME EQUALS ಕೀಲಿ_ಸಂಕೇತ_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

ಕೀಲಿ_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = ಕೀಲಿ_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.ತಲೆ = $1.ತಲೆ; $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$3->common; $$.ಕೊನೆಯ = $3; }
                |       VModDef
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, ಖಾಲಿ); }
                |       Ident EQUALS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET Interpret_ಹೊಂದಿಕೆ OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.ತಲೆ; $$ = $2; }
                ;

Interpret_ಹೊಂದಿಕೆ  :       ಕೀಲಿ_Sym_2 PLUS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = InterpCreate($1, $3); }
                |       ಕೀಲಿ_Sym_2
                        { $$ = InterpCreate($1, ಖಾಲಿ); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            ಆದರೆ ($2) {
                                ಆದರೆ ($1.ತಲೆ) {
                                    $$.ತಲೆ = $1.ತಲೆ;
                                    $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$2->common;
                                    $$.ಕೊನೆಯ = $2;
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    $$.ತಲೆ = $$.ಕೊನೆಯ = $2;
                                }
                            }
                        }
                |       { $$.ತಲೆ = $$.ಕೊನೆಯ = ಖಾಲಿ; }
                ;

ಕೀಲಿ_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = ಕೀಲಿ_Type_Create($2, $4.ತಲೆ); }
                ;

SymbolsDecl     :       ಕೀಲಿ_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.ತಲೆ); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.ತಲೆ = $$.ಕೊನೆಯ = ಖಾಲಿ; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.ತಲೆ = $1.ತಲೆ; $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$3->common; $$.ಕೊನೆಯ = $3; }
                |       SymbolsVarDecl
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS ಅಭಿವ್ಯಕ್ತಿ_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_ಕೀಲಿ_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_ಕೀಲಿ_Sym_Or_Action_List { $$ = VarCreate(ಖಾಲಿ, $1); }
                ;

/*
 * A list of keysym/action lists
 *
 * There is some ambiguity because we use `{}` to denote both an empty list of
 * keysyms and an empty list of actions. But as soon as we get a keysym or an
 * action, we know whether it is a `MultiKeySymList` or a `MultiActionList`.
 * So we just count the `{}` at the *beginning* using `NoSymbolOrActionList`,
 * then replace it by the relevant count of `NoSymbol` or `NoAction()` once the
 * ambiguity is solved. If not, this is a list of empties of *some* type: we
 * drop those empties and delegate the type resolution using `ExprEmptyList()`.
 */
Multi_ಕೀಲಿ_Sym_Or_Action_List
                :       OBRACKET Multi_ಕೀಲಿ_Sym_List CBRACKET
                        { $$ = $2.ತಲೆ; }
                |       OBRACKET No_ಚಿಹ್ನೆ_Or_Action_List COMMA Multi_ಕೀಲಿ_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            ರಚನೆ {ಅಭಿವ್ಯಕ್ತಿ_Def *ತಲೆ; ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೊನೆಯ;} list = {
                                .ತಲೆ = $4.ತಲೆ, .ಕೊನೆಯ = $4.ಕೊನೆಯ
                            };
                            ಗಾಗಿ (uint32_t k = 0; k < $2; k++) {
                                ಅಭಿವ್ಯಕ್ತಿ_Def* ಸ್ಥಿರಾಂಕ syms =
                                    ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                                ಆದರೆ (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.ಮುಂದಿನ = &list.ತಲೆ->common;
                                list.ತಲೆ = syms;
                            }
                            $$ = list.ತಲೆ;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.ತಲೆ; }
                |       OBRACKET No_ಚಿಹ್ನೆ_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            ರಚನೆ {ಅಭಿವ್ಯಕ್ತಿ_Def *ತಲೆ; ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೊನೆಯ;} list = {
                                .ತಲೆ = $4.ತಲೆ, .ಕೊನೆಯ = $4.ಕೊನೆಯ
                            };
                            ಗಾಗಿ (uint32_t k = 0; k < $2; k++) {
                                ಅಭಿವ್ಯಕ್ತಿ_Def* ಸ್ಥಿರಾಂಕ acts = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಖಾಲಿ);
                                ಆದರೆ (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.ಮುಂದಿನ = &list.ತಲೆ->common;
                                list.ತಲೆ = acts;
                            }
                            $$ = list.ತಲೆ;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_ಚಿಹ್ನೆ_Or_Action_List CBRACKET
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_ಚಿಹ್ನೆ_Or_Action_List
                :       No_ಚಿಹ್ನೆ_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

ಗುಂಪು_Compat_Decl :       ಗುಂಪು_2 Integer EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = ಗುಂಪು_Compat_Create($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP ಅಭಿವ್ಯಕ್ತಿ_2 OBRACE ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List CBRACE SEMI
                        {
                            ಆದರೆ (param->config.format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 &&
                                $2->common.ಪ್ರಕಾರ != STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ಮುಖವಾಡ,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.ತಲೆ);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.ತಲೆ);
                        }
                ;

ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List :       ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Or_ಕೀಲಿ_Sym
                        { $$.ತಲೆ = $1.ತಲೆ; $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$3->common; $$.ಕೊನೆಯ = $3; }
                |       ಕೀಲಿ_Or_ಕೀಲಿ_Sym
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = $1; }
                ;

ಕೀಲಿ_Or_ಕೀಲಿ_Sym     :       KEYNAME
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name($1); }
                |       ಕೀಲಿ_Sym_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.ತಲೆ); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        {
                            FreeStmt((ParseCommon *) $2);
                            FreeStmt((ParseCommon *) $4);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, $1);
                        }
                ;

UnknownCompoundStatementDecl:
                        IDENT OptTerminal OBRACE VarDeclList CBRACE SEMI
                        {
                            FreeStmt((ParseCommon *) $2);
                            FreeStmt((ParseCommon *) $4.ತಲೆ);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = ಖಾಲಿ; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (ಶೂನ್ಯ) $4; $$ = ಖಾಲಿ; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ಖಾಲಿ; }
                ;

SectionBody     :       SectionBody Section_Body_ಅಂಶ     { $$ = ಖಾಲಿ;}
                |       Section_Body_ಅಂಶ                 { $$ = ಖಾಲಿ; }
                ;

Section_Body_ಅಂಶ :       ಅಡ್ಡಸಾಲು OBRACE ಅಡ್ಡಸಾಲು_Body CBRACE SEMI
                        { $$ = ಖಾಲಿ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ಖಾಲಿ; }
                |       DoodadDecl
                        { $$ = ಖಾಲಿ; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ಖಾಲಿ; }
                |       OverlayDecl
                        { $$ = ಖಾಲಿ; }
                ;

ಅಡ್ಡಸಾಲು_Body         :       ಅಡ್ಡಸಾಲು_Body ಅಡ್ಡಸಾಲು_Body_ಅಂಶ     { $$ = ಖಾಲಿ;}
                |       ಅಡ್ಡಸಾಲು_Body_ಅಂಶ             { $$ = ಖಾಲಿ; }
                ;

ಅಡ್ಡಸಾಲು_Body_ಅಂಶ     :       ಕೀಲಿಗಳು_2 OBRACE ಕೀಲಿಗಳು_3 CBRACE SEMI { $$ = ಖಾಲಿ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ಖಾಲಿ; }
                ;

ಕೀಲಿಗಳು_3            :       ಕೀಲಿಗಳು_3 COMMA ಕೀಲಿ_4          { $$ = ಖಾಲಿ; }
                |       ಕೀಲಿ_4                     { $$ = ಖಾಲಿ; }
                ;

ಕೀಲಿ_4             :       KEYNAME
                        { $$ = ಖಾಲಿ; }
                |       OBRACE ಅಭಿವ್ಯಕ್ತಿ_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.ತಲೆ); $$ = ಖಾಲಿ; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_ಕೀಲಿ_List CBRACE SEMI
                        { $$ = ಖಾಲಿ; }
                ;

Overlay_ಕೀಲಿ_List  :       Overlay_ಕೀಲಿ_List COMMA Overlay_ಕೀಲಿ { $$ = ಖಾಲಿ; }
                |       Overlay_ಕೀಲಿ                      { $$ = ಖಾಲಿ; }
                ;

Overlay_ಕೀಲಿ      :       KEYNAME EQUALS KEYNAME          { $$ = ಖಾಲಿ; }
                ;

OutlineList     :       OutlineList COMMA Outline_ಒಳ_List
                        { $$ = ಖಾಲಿ;}
                |       Outline_ಒಳ_List
                        { $$ = ಖಾಲಿ; }
                ;

Outline_ಒಳ_List   :       OBRACE CoordList CBRACE
                        { (ಶೂನ್ಯ) $2; $$ = ಖಾಲಿ; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (ಶೂನ್ಯ) $4; $$ = ಖಾಲಿ; }
                |       Ident EQUALS ಅಭಿವ್ಯಕ್ತಿ_2
                        { FreeStmt((ParseCommon *) $3); $$ = ಖಾಲಿ; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (ಶೂನ್ಯ) $1; (ಶೂನ್ಯ) $3; $$ = ಖಾಲಿ; }
                |       Coord
                        { (ಶೂನ್ಯ) $1; $$ = ಖಾಲಿ; }
                ;

Coord           :       OBRACKET Signed_ಸಂಖ್ಯೆ COMMA Signed_ಸಂಖ್ಯೆ CBRACKET
                        { $$ = ಖಾಲಿ; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.ತಲೆ); $$ = ಖಾಲಿ; }
                ;

DoodadType      :       ಪಠ್ಯ    { $$ = 0; }
                |       OUTLINE { $$ = 0; }
                |       SOLID   { $$ = 0; }
                |       LOGO    { $$ = 0; }
                ;

ಕ್ಷೇತ್ರ_Spec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       ಕೀಲಿ_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       ಗುಂಪು_2
                        { $$ = xkb_atom_intern_literal(param->ctx, "group"); }
                |       MODIFIER_MAP
                        {$$ = xkb_atom_intern_literal(param->ctx, "modifier_map");}
                |       INDICATOR
                        { $$ = xkb_atom_intern_literal(param->ctx, "indicator"); }
                |       SHAPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "shape"); }
                |       ಅಡ್ಡಸಾಲು
                        { $$ = xkb_atom_intern_literal(param->ctx, "row"); }
                |       SECTION
                        { $$ = xkb_atom_intern_literal(param->ctx, "section"); }
                |       ಪಠ್ಯ
                        { $$ = xkb_atom_intern_literal(param->ctx, "text"); }
                ;

Opt_ವಿಲೀನ_ಬಹುಲಕ    :       ವಿಲೀನ_ಬಹುಲಕ_2       { $$ = $1; }
                |                       { $$ = ವಿಲೀನ_DEFAULT; }
                ;

ವಿಲೀನ_ಬಹುಲಕ_2       :       INCLUDE         { $$ = ವಿಲೀನ_DEFAULT; }
                |       AUGMENT         { $$ = ವಿಲೀನ_AUGMENT; }
                |       OVERRIDE        { $$ = ವಿಲೀನ_OVERRIDE; }
                |       REPLACE         { $$ = ವಿಲೀನ_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = ವಿಲೀನ_DEFAULT;
                }
                ;

ಅಭಿವ್ಯಕ್ತಿ_List_2        :       ಅಭಿವ್ಯಕ್ತಿ_List_2 COMMA ಅಭಿವ್ಯಕ್ತಿ_2
                        {
                            ಆದರೆ ($3) {
                                ಆದರೆ ($1.ತಲೆ) {
                                    $$.ತಲೆ = $1.ತಲೆ;
                                    $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$3->common;
                                    $$.ಕೊನೆಯ = $3;
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    $$.ತಲೆ = $$.ಕೊನೆಯ = $3;
                                }
                            }
                        }
                |       ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = $1; }
                |       { $$.ತಲೆ = $$.ಕೊನೆಯ = ಖಾಲಿ; }
                ;

ಅಭಿವ್ಯಕ್ತಿ_2            :       ಅಭಿವ್ಯಕ್ತಿ_2 DIVIDE ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE, $1, $3); }
                |       ಅಭಿವ್ಯಕ್ತಿ_2 PLUS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು, $1, $3); }
                |       ಅಭಿವ್ಯಕ್ತಿ_2 MINUS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ, $1, $3); }
                |       ಅಭಿವ್ಯಕ್ತಿ_2 TIMES ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY, $1, $3); }
                |       Lhs EQUALS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು, $1, $3); }
                |       ಪದ
                        { $$ = $1; }
                ;

ಪದ            :       MINUS ಪದ
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE, $2); }
                |       PLUS ಪದ
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS, $2); }
                |       EXCLAM ಪದ
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT, $2); }
                |       INVERT ಪದ
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       ಕ್ಷೇತ್ರ_Spec OPAREN ಅಭಿವ್ಯಕ್ತಿ_List_2 CPAREN %prec OPAREN
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action($1, $3.ತಲೆ); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN ಅಭಿವ್ಯಕ್ತಿ_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List($3);
                            $$ = $1;
                            $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &ಅಭಿವ್ಯಕ್ತಿ->common; $$.ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$3->common; $$.ಕೊನೆಯ = $3; }
                |       Action
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List($1); }
                |       Non_ಖಾಲಿಗಳು_Actions
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$3->common; $$.ಕೊನೆಯ = $3; }
                |       Action
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = $1; }
                ;

Non_ಖಾಲಿಗಳು_Actions :       OBRACE ActionList CBRACE
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List($2.ತಲೆ); }
                ;

Actions         :       Non_ಖಾಲಿಗಳು_Actions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ಖಾಲಿ); }
                ;

Action          :       ಕ್ಷೇತ್ರ_Spec OPAREN ಅಭಿವ್ಯಕ್ತಿ_List_2 CPAREN
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action($1, $3.ತಲೆ); }
                ;

Lhs             :       ಕ್ಷೇತ್ರ_Spec
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Ident($1); }
                |       ಕ್ಷೇತ್ರ_Spec DOT ಕ್ಷೇತ್ರ_Spec
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕ್ಷೇತ್ರ_Ref($1, $3); }
                |       ಕ್ಷೇತ್ರ_Spec OBRACKET ಅಭಿವ್ಯಕ್ತಿ_2 CBRACKET
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       ಕ್ಷೇತ್ರ_Spec DOT ಕ್ಷೇತ್ರ_Spec OBRACKET ಅಭಿವ್ಯಕ್ತಿ_2 CBRACKET
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ಖಾಲಿ; }
                ;

Terminal        :       String
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_String($1); }
                |       Integer
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Integer($1); }
                |       Float
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name($1); }
                ;

Multi_ಕೀಲಿ_Sym_List :       Multi_ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Sym_Lit
                        {
                            ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List($3);
                            $$ = $1;
                            $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &ಅಭಿವ್ಯಕ್ತಿ->common; $$.ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ;
                        }
                |       Multi_ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Syms
                        { $$ = $1; $$.ಕೊನೆಯ->common.ಮುಂದಿನ = &$3->common; $$.ಕೊನೆಯ = $3; }
                |       ಕೀಲಿ_Sym_Lit
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List($1); }
                |       Non_ಖಾಲಿಗಳು_ಕೀಲಿ_Syms
                        { $$.ತಲೆ = $$.ಕೊನೆಯ = $1; }
                ;

ಕೀಲಿ_Sym_List      :       ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Sym_Lit
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Append_ಕೀಲಿ_Sym_List($1, $3); }
                |       ಕೀಲಿ_Sym_List COMMA STRING
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, $1, $3);
                            ಬಿಡುಗಡೆ($3);
                            ಆದರೆ (!$$)
                                YYERROR;
                        }
                |       ಕೀಲಿ_Sym_Lit
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List($1);
                            ಆದರೆ (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                            ಆದರೆ (!$$)
                                YYERROR;
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, $$, $1);
                            ಬಿಡುಗಡೆ($1);
                            ಆದರೆ (!$$)
                                YYERROR;
                        }
                ;

Non_ಖಾಲಿಗಳು_ಕೀಲಿ_Syms :       OBRACE ಕೀಲಿ_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                            ಆದರೆ (!$$)
                                YYERROR;
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, $$, $1);
                            ಬಿಡುಗಡೆ($1);
                            ಆದರೆ (!$$)
                                YYERROR;
                        }
                ;

ಕೀಲಿ_Syms         :       Non_ಖಾಲಿಗಳು_ಕೀಲಿ_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ); }
                ;

ಕೀಲಿ_Sym_2          :       ಕೀಲಿ_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            ಬಿಡುಗಡೆ($1);
                            ಆದರೆ ($$ == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                                YYERROR;
                        }
                ;

ಕೀಲಿ_Sym_Lit       :       IDENT
                        {
                            ಆದರೆ (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) $1.len, $1.ಆರಂಭ
                                );
                                $$ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_ಕೀಲಿ_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_ಕೀಲಿ_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            ಆದರೆ ($1 < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -$1, $1
                                );
                                $$ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
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
                                ಆದರೆ ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ಖಾಲಿ, $$, "%#06"PRIx32, "");
                                } ಇಲ್ಲದಿದ್ದರೆ {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
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
                                    $1, $1
                                );
                            }
                        }
                ;

Signed_ಸಂಖ್ಯೆ    :       MINUS ಸಂಖ್ಯೆ    { $$ = -$2; }
                |       ಸಂಖ್ಯೆ          { $$ = $1; }
                ;

ಸಂಖ್ಯೆ          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

ಕೀಲಿ_ಸಂಕೇತ_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.ಆರಂಭ, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ($1)); ಬಿಡುಗಡೆ($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ಖಾಲಿ; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

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
