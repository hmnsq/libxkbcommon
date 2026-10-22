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
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/xkbcomp-priv.h"
}

%{
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
    ਅੱਖਰ buf[XKB_KEYSYM_NAME_MAX_SIZE];
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
%}

%define api.ਸ਼ੁੱਧ
%lex-param      { ਬਣਤਰ scanner *param_scanner }
%parse-param    { ਬਣਤਰ parser_param *param }

%define parse.error detailed

%token
        ਸਿਰਾ_ਦਾ_FILE     0 "end of file"
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
        ਕੁੰਜੀ_3             24 "key"
        ALIAS           25 "alias"
        ਸਮੂਹ_2           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        ਕੁੰਜੀਆਂ_2            30 "keys"
        ਪੰਕਤੀ             31 "row"
        SECTION         32 "section"
        OVERLAY         33 "overlay"
        ਪਾਠ            34 "text"
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
        ALPHANUMERIC_ਕੁੰਜੀਆਂ       73 "alphanumeric_keys"
        MODIFIER_ਕੁੰਜੀਆਂ           74 "modifier_keys"
        KEYPAD_ਕੁੰਜੀਆਂ             75 "keypad_keys"
        FUNCTION_ਕੁੰਜੀਆਂ           76 "function_keys"
        ALTERNATE_ਸਮੂਹ         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%ਸੱਜੇ  EQUALS
%ਖੱਬੇ   PLUS MINUS
%ਖੱਬੇ   TIMES DIVIDE
%ਖੱਬੇ   EXCLAM INVERT
%ਖੱਬੇ   OPAREN

%start  XkbFile

%ਸੰਘ  {
        int64_t          ਅੰਕ;
        ਗਿਣਤੀ xkb_file_type file_type;
        ਅੱਖਰ            *str;
        ਬਣਤਰ sval     sval;
        xkb_atom_t      atom;
        ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
        ਗਿਣਤੀ xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        ਬਣਤਰ { ParseCommon *ਸਿਰ; ParseCommon *ਆਖਰੀ; } anyList;
        uint32_t        no_ਚਿੰਨ੍ਹ_Or_Action_List;
        ਵਿਅੰਜਕ_Def         *ਵਿਅੰਜਕ;
        ਬਣਤਰ { ਵਿਅੰਜਕ_Def *ਸਿਰ; ਵਿਅੰਜਕ_Def *ਆਖਰੀ; } ਵਿਅੰਜਕ_List;
        VarDef          *var;
        ਬਣਤਰ { VarDef *ਸਿਰ; VarDef *ਆਖਰੀ; } varList;
        VModDef         *vmod;
        ਬਣਤਰ { VModDef *ਸਿਰ; VModDef *ਆਖਰੀ; } vmodList;
        InterpDef       *interp;
        ਕੁੰਜੀ_Type_Def      *ਕੁੰਜੀ_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_ਨਕਾਬ_2;
        ਸਮੂਹ_Compat_Def  *ਸਮੂਹ_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *ਕੁੰਜੀ_ਕੋਡ;
        ਕੁੰਜੀ_Alias_Def     *ਕੁੰਜੀ_Alias;
        UnknownStatement *unknown;
        ਖਾਲੀ            *geom;
        XkbFile         *file;
        ਬਣਤਰ { XkbFile *ਸਿਰ; XkbFile *ਆਖਰੀ; } fileList;
}

%ਕਿਸਮ <ਅੰਕ>     DECIMAL_DIGIT INTEGER FLOAT
%ਕਿਸਮ <str>     STRING
%ਕਿਸਮ <sval>    IDENT
%ਕਿਸਮ <atom>    KEYNAME
%ਕਿਸਮ <ਅੰਕ>     ਕੁੰਜੀ_ਕੋਡ_2 ਸੰਖਿਆ Integer Float Signed_ਸੰਖਿਆ DoodadType
%ਕਿਸਮ <ਮਿਲਾਓ>   ਮਿਲਾਓ_ਬਹੁਲਕ_2 Opt_ਮਿਲਾਓ_ਬਹੁਲਕ
%ਕਿਸਮ <file_type> Xkb_ਸੰਯੁਕਤ_Type FileType
%ਕਿਸਮ <mapFlags> Flag Flags OptFlags
%ਕਿਸਮ <str>     MapName OptMapName
%ਕਿਸਮ <atom>    ਖੇਤਰ_Spec Ident Element String
%ਕਿਸਮ <keysym>  ਕੁੰਜੀ_Sym_2 ਕੁੰਜੀ_Sym_Lit
%ਕਿਸਮ <no_ਚਿੰਨ੍ਹ_Or_Action_List> No_ਚਿੰਨ੍ਹ_Or_Action_List
%ਕਿਸਮ <any>     Decl
%ਕਿਸਮ <anyList> DeclList
%ਕਿਸਮ <ਵਿਅੰਜਕ>    ਵਿਅੰਜਕ_2 ਪਦ Lhs Terminal OptTerminal Coord CoordList
%ਕਿਸਮ <ਵਿਅੰਜਕ>    Multi_ਕੁੰਜੀ_Sym_Or_Action_List Non_ਖਾਲੀ_ਥਾਵਾਂ_Actions Actions Action
%ਕਿਸਮ <ਵਿਅੰਜਕ>    ਕੁੰਜੀ_Syms Non_ਖਾਲੀ_ਥਾਵਾਂ_ਕੁੰਜੀ_Syms ਕੁੰਜੀ_Sym_List ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym
%ਕਿਸਮ <ਵਿਅੰਜਕ_List> ਵਿਅੰਜਕ_List_2 ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym_List
%ਕਿਸਮ <ਵਿਅੰਜਕ_List> ActionList MultiActionList Multi_ਕੁੰਜੀ_Sym_List
%ਕਿਸਮ <var>     VarDecl SymbolsVarDecl
%ਕਿਸਮ <varList> VarDeclList SymbolsBody OptSymbolsBody
%ਕਿਸਮ <vmod>    VModDef
%ਕਿਸਮ <vmodList> VModDefList VModDecl
%ਕਿਸਮ <interp>  InterpretDecl Interpret_ਮੇਲ
%ਕਿਸਮ <ਕੁੰਜੀ_Type> ਕੁੰਜੀ_Type_Decl
%ਕਿਸਮ <syms>    SymbolsDecl
%ਕਿਸਮ <mod_ਨਕਾਬ_2> ModMapDecl
%ਕਿਸਮ <ਸਮੂਹ_Compat> ਸਮੂਹ_Compat_Decl
%ਕਿਸਮ <ledMap>  LedMapDecl
%ਕਿਸਮ <ledName> LedNameDecl
%ਕਿਸਮ <ਕੁੰਜੀ_ਕੋਡ> ਕੁੰਜੀ_Name_Decl
%ਕਿਸਮ <ਕੁੰਜੀ_Alias> ਕੁੰਜੀ_Alias_Decl
%ਕਿਸਮ <geom>    ShapeDecl SectionDecl SectionBody Section_Body_ਵਸਤੂ ਪੰਕਤੀ_Body ਪੰਕਤੀ_Body_ਵਸਤੂ
%ਕਿਸਮ <geom>    ਕੁੰਜੀਆਂ_3 ਕੁੰਜੀ_4 OverlayDecl Overlay_ਕੁੰਜੀ_List Overlay_ਕੁੰਜੀ OutlineList Outline_ਵਿੱਚ_List
%ਕਿਸਮ <geom>    DoodadDecl
%ਕਿਸਮ <unknown> UnknownDecl UnknownCompoundStatementDecl
%ਕਿਸਮ <file>    XkbFile XkbMapConfig
%ਕਿਸਮ <fileList> XkbMapConfigList
%ਕਿਸਮ <file>    Xkb_ਸੰਯੁਕਤ_Map

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <ਵਿਅੰਜਕ> <var> <vmod> <interp> <ਕੁੰਜੀ_Type> <syms> <mod_ਨਕਾਬ_2> <ਸਮੂਹ_Compat>
    <ledMap> <ledName> <ਕੁੰਜੀ_ਕੋਡ> <ਕੁੰਜੀ_Alias>
%destructor { FreeStmt((ParseCommon *) $$.ਸਿਰ); }
    <anyList> <ਵਿਅੰਜਕ_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { ਜੇ (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.ਸਿਰ); } <fileList>
%destructor { ਮੁਕਤ($$); } <str>

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

XkbFile         :       Xkb_ਸੰਯੁਕਤ_Map
                        { $$ = param->rtrn = $1; param->ਹੋਰ_maps = !!param->rtrn; (ਖਾਲੀ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->ਹੋਰ_maps = !!param->rtrn; YYACCEPT; }
                |       ਸਿਰਾ_ਦਾ_FILE
                        { $$ = param->rtrn = ਨਿਰਾ; param->ਹੋਰ_maps = false; }
                ;

Xkb_ਸੰਯੁਕਤ_Map :       OptFlags Xkb_ਸੰਯੁਕਤ_Type OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.ਸਿਰ, $1); }
                ;

Xkb_ਸੰਯੁਕਤ_Type:       XKB_KEYMAP      { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_SEMANTICS   { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_LAYOUT      { $$ = FILE_TYPE_KEYMAP; }
                ;

/*
 * NOTE: Any component is optional
 */
XkbMapConfigList :      XkbMapConfigList XkbMapConfig
                        {
                            ਜੇ ($2) {
                                ਜੇ ($1.ਸਿਰ) {
                                    $$.ਸਿਰ = $1.ਸਿਰ;
                                    $$.ਆਖਰੀ->common.ਅਗਲਾ = &$2->common;
                                    $$.ਆਖਰੀ = $2;
                                } ਨਹੀਂਤਾਂ {
                                    $$.ਸਿਰ = $$.ਆਖਰੀ = $2;
                                }
                            }
                        }
                |       { $$.ਸਿਰ = $$.ਆਖਰੀ = ਨਿਰਾ; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            ਜੇ ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            ਸੁਰੱਖਿਅਤ_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.ਸਿਰ, $1);
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
                |       ALPHANUMERIC_ਕੁੰਜੀਆਂ       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_ਕੁੰਜੀਆਂ           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_ਕੁੰਜੀਆਂ             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_ਕੁੰਜੀਆਂ           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_ਸਮੂਹ         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ਅਚਲ bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_ਕੋਡ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (ਬਿਨਾਨਿਸ਼ਾਨੀ)$1.len, $1.ਸ਼ੁਰੂ,
                                (error ? "" : "; ignored")
                            );
                            ਜੇ (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            ਜੇ ($2) {
                                ਜੇ ($1.ਸਿਰ) {
                                    $$.ਸਿਰ = $1.ਸਿਰ; $1.ਆਖਰੀ->ਅਗਲਾ = $2; $$.ਆਖਰੀ = $2;
                                } ਨਹੀਂਤਾਂ {
                                    $$.ਸਿਰ = $$.ਆਖਰੀ = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList Opt_ਮਿਲਾਓ_ਬਹੁਲਕ VModDecl
                        {
                            ਲਈ (VModDef *vmod = $3.ਸਿਰ; vmod; vmod = (VModDef *) vmod->common.ਅਗਲਾ)
                                vmod->ਮਿਲਾਓ = $2;
                            ਜੇ ($1.ਸਿਰ) {
                                $$.ਸਿਰ = $1.ਸਿਰ; $1.ਆਖਰੀ->ਅਗਲਾ = &$3.ਸਿਰ->common; $$.ਆਖਰੀ = &$3.ਆਖਰੀ->common;
                            } ਨਹੀਂਤਾਂ {
                                $$.ਸਿਰ = &$3.ਸਿਰ->common; $$.ਆਖਰੀ = &$3.ਆਖਰੀ->common;
                            }
                        }
                |       { $$.ਸਿਰ = $$.ਆਖਰੀ = ਨਿਰਾ; }
                ;

Decl            :       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ VarDecl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ InterpretDecl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ ਕੁੰਜੀ_Name_Decl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ ਕੁੰਜੀ_Alias_Decl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ ਕੁੰਜੀ_Type_Decl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ SymbolsDecl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ ModMapDecl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ ਸਮੂਹ_Compat_Decl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ LedMapDecl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ LedNameDecl
                        {
                            $2->ਮਿਲਾਓ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ ShapeDecl          { $$ = ਨਿਰਾ; }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ SectionDecl        { $$ = ਨਿਰਾ; }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ DoodadDecl         { $$ = ਨਿਰਾ; }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       Opt_ਮਿਲਾਓ_ਬਹੁਲਕ UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       ਮਿਲਾਓ_ਬਹੁਲਕ_2 STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            ਮੁਕਤ($2);
                        }
                ;

VarDecl         :       Lhs EQUALS ਵਿਅੰਜਕ_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

ਕੁੰਜੀ_Name_Decl     :       KEYNAME EQUALS ਕੁੰਜੀ_ਕੋਡ_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

ਕੁੰਜੀ_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = ਕੁੰਜੀ_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.ਸਿਰ = $1.ਸਿਰ; $$.ਆਖਰੀ->common.ਅਗਲਾ = &$3->common; $$.ਆਖਰੀ = $3; }
                |       VModDef
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, ਨਿਰਾ); }
                |       Ident EQUALS ਵਿਅੰਜਕ_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET Interpret_ਮੇਲ OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.ਸਿਰ; $$ = $2; }
                ;

Interpret_ਮੇਲ  :       ਕੁੰਜੀ_Sym_2 PLUS ਵਿਅੰਜਕ_2
                        { $$ = InterpCreate($1, $3); }
                |       ਕੁੰਜੀ_Sym_2
                        { $$ = InterpCreate($1, ਨਿਰਾ); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            ਜੇ ($2) {
                                ਜੇ ($1.ਸਿਰ) {
                                    $$.ਸਿਰ = $1.ਸਿਰ;
                                    $$.ਆਖਰੀ->common.ਅਗਲਾ = &$2->common;
                                    $$.ਆਖਰੀ = $2;
                                } ਨਹੀਂਤਾਂ {
                                    $$.ਸਿਰ = $$.ਆਖਰੀ = $2;
                                }
                            }
                        }
                |       { $$.ਸਿਰ = $$.ਆਖਰੀ = ਨਿਰਾ; }
                ;

ਕੁੰਜੀ_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = ਕੁੰਜੀ_Type_Create($2, $4.ਸਿਰ); }
                ;

SymbolsDecl     :       ਕੁੰਜੀ_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.ਸਿਰ); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.ਸਿਰ = $$.ਆਖਰੀ = ਨਿਰਾ; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.ਸਿਰ = $1.ਸਿਰ; $$.ਆਖਰੀ->common.ਅਗਲਾ = &$3->common; $$.ਆਖਰੀ = $3; }
                |       SymbolsVarDecl
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS ਵਿਅੰਜਕ_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_ਕੁੰਜੀ_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_ਕੁੰਜੀ_Sym_Or_Action_List { $$ = VarCreate(ਨਿਰਾ, $1); }
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
Multi_ਕੁੰਜੀ_Sym_Or_Action_List
                :       OBRACKET Multi_ਕੁੰਜੀ_Sym_List CBRACKET
                        { $$ = $2.ਸਿਰ; }
                |       OBRACKET No_ਚਿੰਨ੍ਹ_Or_Action_List COMMA Multi_ਕੁੰਜੀ_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            ਬਣਤਰ {ਵਿਅੰਜਕ_Def *ਸਿਰ; ਵਿਅੰਜਕ_Def *ਆਖਰੀ;} list = {
                                .ਸਿਰ = $4.ਸਿਰ, .ਆਖਰੀ = $4.ਆਖਰੀ
                            };
                            ਲਈ (uint32_t k = 0; k < $2; k++) {
                                ਵਿਅੰਜਕ_Def* ਅਚਲ syms =
                                    ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
                                ਜੇ (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.ਅਗਲਾ = &list.ਸਿਰ->common;
                                list.ਸਿਰ = syms;
                            }
                            $$ = list.ਸਿਰ;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.ਸਿਰ; }
                |       OBRACKET No_ਚਿੰਨ੍ਹ_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            ਬਣਤਰ {ਵਿਅੰਜਕ_Def *ਸਿਰ; ਵਿਅੰਜਕ_Def *ਆਖਰੀ;} list = {
                                .ਸਿਰ = $4.ਸਿਰ, .ਆਖਰੀ = $4.ਆਖਰੀ
                            };
                            ਲਈ (uint32_t k = 0; k < $2; k++) {
                                ਵਿਅੰਜਕ_Def* ਅਚਲ acts = ਵਿਅੰਜਕ_Create_Action_List(ਨਿਰਾ);
                                ਜੇ (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.ਅਗਲਾ = &list.ਸਿਰ->common;
                                list.ਸਿਰ = acts;
                            }
                            $$ = list.ਸਿਰ;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_ਚਿੰਨ੍ਹ_Or_Action_List CBRACKET
                        { $$ = ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_ਚਿੰਨ੍ਹ_Or_Action_List
                :       No_ਚਿੰਨ੍ਹ_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

ਸਮੂਹ_Compat_Decl :       ਸਮੂਹ_2 Integer EQUALS ਵਿਅੰਜਕ_2 SEMI
                        { $$ = ਸਮੂਹ_Compat_Create($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP ਵਿਅੰਜਕ_2 OBRACE ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym_List CBRACE SEMI
                        {
                            ਜੇ (param->config.format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1 &&
                                $2->common.ਕਿਸਮ != STMT_ਵਿਅੰਜਕ_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ਨਕਾਬ,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.ਸਿਰ);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.ਸਿਰ);
                        }
                ;

ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym_List :       ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym_List COMMA ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym
                        { $$.ਸਿਰ = $1.ਸਿਰ; $$.ਆਖਰੀ->common.ਅਗਲਾ = &$3->common; $$.ਆਖਰੀ = $3; }
                |       ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = $1; }
                ;

ਕੁੰਜੀ_Or_ਕੁੰਜੀ_Sym     :       KEYNAME
                        { $$ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Name($1); }
                |       ਕੁੰਜੀ_Sym_2
                        { $$ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.ਸਿਰ); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS ਵਿਅੰਜਕ_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS ਵਿਅੰਜਕ_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS ਵਿਅੰਜਕ_2 SEMI
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
                            FreeStmt((ParseCommon *) $4.ਸਿਰ);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = ਨਿਰਾ; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (ਖਾਲੀ) $4; $$ = ਨਿਰਾ; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ਨਿਰਾ; }
                ;

SectionBody     :       SectionBody Section_Body_ਵਸਤੂ     { $$ = ਨਿਰਾ;}
                |       Section_Body_ਵਸਤੂ                 { $$ = ਨਿਰਾ; }
                ;

Section_Body_ਵਸਤੂ :       ਪੰਕਤੀ OBRACE ਪੰਕਤੀ_Body CBRACE SEMI
                        { $$ = ਨਿਰਾ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ਨਿਰਾ; }
                |       DoodadDecl
                        { $$ = ਨਿਰਾ; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ਨਿਰਾ; }
                |       OverlayDecl
                        { $$ = ਨਿਰਾ; }
                ;

ਪੰਕਤੀ_Body         :       ਪੰਕਤੀ_Body ਪੰਕਤੀ_Body_ਵਸਤੂ     { $$ = ਨਿਰਾ;}
                |       ਪੰਕਤੀ_Body_ਵਸਤੂ             { $$ = ਨਿਰਾ; }
                ;

ਪੰਕਤੀ_Body_ਵਸਤੂ     :       ਕੁੰਜੀਆਂ_2 OBRACE ਕੁੰਜੀਆਂ_3 CBRACE SEMI { $$ = ਨਿਰਾ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ਨਿਰਾ; }
                ;

ਕੁੰਜੀਆਂ_3            :       ਕੁੰਜੀਆਂ_3 COMMA ਕੁੰਜੀ_4          { $$ = ਨਿਰਾ; }
                |       ਕੁੰਜੀ_4                     { $$ = ਨਿਰਾ; }
                ;

ਕੁੰਜੀ_4             :       KEYNAME
                        { $$ = ਨਿਰਾ; }
                |       OBRACE ਵਿਅੰਜਕ_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.ਸਿਰ); $$ = ਨਿਰਾ; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_ਕੁੰਜੀ_List CBRACE SEMI
                        { $$ = ਨਿਰਾ; }
                ;

Overlay_ਕੁੰਜੀ_List  :       Overlay_ਕੁੰਜੀ_List COMMA Overlay_ਕੁੰਜੀ { $$ = ਨਿਰਾ; }
                |       Overlay_ਕੁੰਜੀ                      { $$ = ਨਿਰਾ; }
                ;

Overlay_ਕੁੰਜੀ      :       KEYNAME EQUALS KEYNAME          { $$ = ਨਿਰਾ; }
                ;

OutlineList     :       OutlineList COMMA Outline_ਵਿੱਚ_List
                        { $$ = ਨਿਰਾ;}
                |       Outline_ਵਿੱਚ_List
                        { $$ = ਨਿਰਾ; }
                ;

Outline_ਵਿੱਚ_List   :       OBRACE CoordList CBRACE
                        { (ਖਾਲੀ) $2; $$ = ਨਿਰਾ; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (ਖਾਲੀ) $4; $$ = ਨਿਰਾ; }
                |       Ident EQUALS ਵਿਅੰਜਕ_2
                        { FreeStmt((ParseCommon *) $3); $$ = ਨਿਰਾ; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (ਖਾਲੀ) $1; (ਖਾਲੀ) $3; $$ = ਨਿਰਾ; }
                |       Coord
                        { (ਖਾਲੀ) $1; $$ = ਨਿਰਾ; }
                ;

Coord           :       OBRACKET Signed_ਸੰਖਿਆ COMMA Signed_ਸੰਖਿਆ CBRACKET
                        { $$ = ਨਿਰਾ; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.ਸਿਰ); $$ = ਨਿਰਾ; }
                ;

DoodadType      :       ਪਾਠ    { $$ = 0; }
                |       OUTLINE { $$ = 0; }
                |       SOLID   { $$ = 0; }
                |       LOGO    { $$ = 0; }
                ;

ਖੇਤਰ_Spec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       ਕੁੰਜੀ_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       ਸਮੂਹ_2
                        { $$ = xkb_atom_intern_literal(param->ctx, "group"); }
                |       MODIFIER_MAP
                        {$$ = xkb_atom_intern_literal(param->ctx, "modifier_map");}
                |       INDICATOR
                        { $$ = xkb_atom_intern_literal(param->ctx, "indicator"); }
                |       SHAPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "shape"); }
                |       ਪੰਕਤੀ
                        { $$ = xkb_atom_intern_literal(param->ctx, "row"); }
                |       SECTION
                        { $$ = xkb_atom_intern_literal(param->ctx, "section"); }
                |       ਪਾਠ
                        { $$ = xkb_atom_intern_literal(param->ctx, "text"); }
                ;

Opt_ਮਿਲਾਓ_ਬਹੁਲਕ    :       ਮਿਲਾਓ_ਬਹੁਲਕ_2       { $$ = $1; }
                |                       { $$ = ਮਿਲਾਓ_DEFAULT; }
                ;

ਮਿਲਾਓ_ਬਹੁਲਕ_2       :       INCLUDE         { $$ = ਮਿਲਾਓ_DEFAULT; }
                |       AUGMENT         { $$ = ਮਿਲਾਓ_AUGMENT; }
                |       OVERRIDE        { $$ = ਮਿਲਾਓ_OVERRIDE; }
                |       REPLACE         { $$ = ਮਿਲਾਓ_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = ਮਿਲਾਓ_DEFAULT;
                }
                ;

ਵਿਅੰਜਕ_List_2        :       ਵਿਅੰਜਕ_List_2 COMMA ਵਿਅੰਜਕ_2
                        {
                            ਜੇ ($3) {
                                ਜੇ ($1.ਸਿਰ) {
                                    $$.ਸਿਰ = $1.ਸਿਰ;
                                    $$.ਆਖਰੀ->common.ਅਗਲਾ = &$3->common;
                                    $$.ਆਖਰੀ = $3;
                                } ਨਹੀਂਤਾਂ {
                                    $$.ਸਿਰ = $$.ਆਖਰੀ = $3;
                                }
                            }
                        }
                |       ਵਿਅੰਜਕ_2
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = $1; }
                |       { $$.ਸਿਰ = $$.ਆਖਰੀ = ਨਿਰਾ; }
                ;

ਵਿਅੰਜਕ_2            :       ਵਿਅੰਜਕ_2 DIVIDE ਵਿਅੰਜਕ_2
                        { $$ = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_DIVIDE, $1, $3); }
                |       ਵਿਅੰਜਕ_2 PLUS ਵਿਅੰਜਕ_2
                        { $$ = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_ਜੋੜੋ, $1, $3); }
                |       ਵਿਅੰਜਕ_2 MINUS ਵਿਅੰਜਕ_2
                        { $$ = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_ਘਟਾਓ, $1, $3); }
                |       ਵਿਅੰਜਕ_2 TIMES ਵਿਅੰਜਕ_2
                        { $$ = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_MULTIPLY, $1, $3); }
                |       Lhs EQUALS ਵਿਅੰਜਕ_2
                        { $$ = ਵਿਅੰਜਕ_Create_ਦੁਆਧਾਰੀ(STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ, $1, $3); }
                |       ਪਦ
                        { $$ = $1; }
                ;

ਪਦ            :       MINUS ਪਦ
                        { $$ = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_NEGATE, $2); }
                |       PLUS ਪਦ
                        { $$ = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_UNARY_PLUS, $2); }
                |       EXCLAM ਪਦ
                        { $$ = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_NOT, $2); }
                |       INVERT ਪਦ
                        { $$ = ਵਿਅੰਜਕ_Create_Unary(STMT_ਵਿਅੰਜਕ_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       ਖੇਤਰ_Spec OPAREN ਵਿਅੰਜਕ_List_2 CPAREN %prec OPAREN
                        { $$ = ਵਿਅੰਜਕ_Create_Action($1, $3.ਸਿਰ); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN ਵਿਅੰਜਕ_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create_Action_List($3);
                            $$ = $1;
                            $$.ਆਖਰੀ->common.ਅਗਲਾ = &ਵਿਅੰਜਕ->common; $$.ਆਖਰੀ = ਵਿਅੰਜਕ;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.ਆਖਰੀ->common.ਅਗਲਾ = &$3->common; $$.ਆਖਰੀ = $3; }
                |       Action
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = ਵਿਅੰਜਕ_Create_Action_List($1); }
                |       Non_ਖਾਲੀ_ਥਾਵਾਂ_Actions
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.ਆਖਰੀ->common.ਅਗਲਾ = &$3->common; $$.ਆਖਰੀ = $3; }
                |       Action
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = $1; }
                ;

Non_ਖਾਲੀ_ਥਾਵਾਂ_Actions :       OBRACE ActionList CBRACE
                        { $$ = ਵਿਅੰਜਕ_Create_Action_List($2.ਸਿਰ); }
                ;

Actions         :       Non_ਖਾਲੀ_ਥਾਵਾਂ_Actions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ਵਿਅੰਜਕ_Create_Action_List(ਨਿਰਾ); }
                ;

Action          :       ਖੇਤਰ_Spec OPAREN ਵਿਅੰਜਕ_List_2 CPAREN
                        { $$ = ਵਿਅੰਜਕ_Create_Action($1, $3.ਸਿਰ); }
                ;

Lhs             :       ਖੇਤਰ_Spec
                        { $$ = ਵਿਅੰਜਕ_Create_Ident($1); }
                |       ਖੇਤਰ_Spec DOT ਖੇਤਰ_Spec
                        { $$ = ਵਿਅੰਜਕ_Create_ਖੇਤਰ_Ref($1, $3); }
                |       ਖੇਤਰ_Spec OBRACKET ਵਿਅੰਜਕ_2 CBRACKET
                        { $$ = ਵਿਅੰਜਕ_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       ਖੇਤਰ_Spec DOT ਖੇਤਰ_Spec OBRACKET ਵਿਅੰਜਕ_2 CBRACKET
                        { $$ = ਵਿਅੰਜਕ_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ਨਿਰਾ; }
                ;

Terminal        :       String
                        { $$ = ਵਿਅੰਜਕ_Create_String($1); }
                |       Integer
                        { $$ = ਵਿਅੰਜਕ_Create_Integer($1); }
                |       Float
                        { $$ = ਵਿਅੰਜਕ_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Name($1); }
                ;

Multi_ਕੁੰਜੀ_Sym_List :       Multi_ਕੁੰਜੀ_Sym_List COMMA ਕੁੰਜੀ_Sym_Lit
                        {
                            ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List($3);
                            $$ = $1;
                            $$.ਆਖਰੀ->common.ਅਗਲਾ = &ਵਿਅੰਜਕ->common; $$.ਆਖਰੀ = ਵਿਅੰਜਕ;
                        }
                |       Multi_ਕੁੰਜੀ_Sym_List COMMA ਕੁੰਜੀ_Syms
                        { $$ = $1; $$.ਆਖਰੀ->common.ਅਗਲਾ = &$3->common; $$.ਆਖਰੀ = $3; }
                |       ਕੁੰਜੀ_Sym_Lit
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List($1); }
                |       Non_ਖਾਲੀ_ਥਾਵਾਂ_ਕੁੰਜੀ_Syms
                        { $$.ਸਿਰ = $$.ਆਖਰੀ = $1; }
                ;

ਕੁੰਜੀ_Sym_List      :       ਕੁੰਜੀ_Sym_List COMMA ਕੁੰਜੀ_Sym_Lit
                        { $$ = ਵਿਅੰਜਕ_Append_ਕੁੰਜੀ_Sym_List($1, $3); }
                |       ਕੁੰਜੀ_Sym_List COMMA STRING
                        {
                            $$ = ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(param->scanner, $1, $3);
                            ਮੁਕਤ($3);
                            ਜੇ (!$$)
                                YYERROR;
                        }
                |       ਕੁੰਜੀ_Sym_Lit
                        {
                            $$ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List($1);
                            ਜੇ (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
                            ਜੇ (!$$)
                                YYERROR;
                            $$ = ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(param->scanner, $$, $1);
                            ਮੁਕਤ($1);
                            ਜੇ (!$$)
                                YYERROR;
                        }
                ;

Non_ਖਾਲੀ_ਥਾਵਾਂ_ਕੁੰਜੀ_Syms :       OBRACE ਕੁੰਜੀ_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
                            ਜੇ (!$$)
                                YYERROR;
                            $$ = ਵਿਅੰਜਕ_ਕੁੰਜੀ_Sym_List_Append_String(param->scanner, $$, $1);
                            ਮੁਕਤ($1);
                            ਜੇ (!$$)
                                YYERROR;
                        }
                ;

ਕੁੰਜੀ_Syms         :       Non_ਖਾਲੀ_ਥਾਵਾਂ_ਕੁੰਜੀ_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ਵਿਅੰਜਕ_Create_ਕੁੰਜੀ_Sym_List(XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ); }
                ;

ਕੁੰਜੀ_Sym_2          :       ਕੁੰਜੀ_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            ਮੁਕਤ($1);
                            ਜੇ ($$ == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ)
                                YYERROR;
                        }
                ;

ਕੁੰਜੀ_Sym_Lit       :       IDENT
                        {
                            ਜੇ (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) $1.len, $1.ਸ਼ੁਰੂ
                                );
                                $$ = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_ਕੁੰਜੀ_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_ਕੁੰਜੀ_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            ਜੇ ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
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
                                ਜੇ ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ਨਿਰਾ, $$, "%#06"PRIx32, "");
                                } ਨਹੀਂਤਾਂ {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
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

Signed_ਸੰਖਿਆ    :       MINUS ਸੰਖਿਆ    { $$ = -$2; }
                |       ਸੰਖਿਆ          { $$ = $1; }
                ;

ਸੰਖਿਆ          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

ਕੁੰਜੀ_ਕੋਡ_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.ਸ਼ੁਰੂ, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, ਲੜੀ_ਲੰਬਾਈ($1)); ਮੁਕਤ($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ਨਿਰਾ; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

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
