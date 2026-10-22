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
#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "scanner-utils.h"
#അന്തര്ഭാവ "xkbcomp/ast.h"
#അന്തര്ഭാവ "xkbcomp/xkbcomp-priv.h"
}

%{
#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "xkbcomp/xkbcomp-priv.h"
#അന്തര്ഭാവ "xkbcomp/parser-priv.h"
#അന്തര്ഭാവ "xkbcomp/ast-build.h"

ഘടന parser_param {
    ഘടന xkb_context *ctx;
    ഘടന scanner *scanner;
    XkbFile *rtrn;
    ഘടന parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_സന്ദേശം_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_സന്ദേശം_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

സ്ഥിതം ശൂന്യം
_xkbcommon_error(ഘടന parser_param *param, സ്ഥിരം അക്ഷരം *സന്ദേശം)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", സന്ദേശം);
}

സ്ഥിതം bool
resolve_keysym(ഘടന parser_param *param, ഘടന sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    എങ്കിൽ (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_താക്കോൽ_No_ചിഹ്നം;
        തിരികെ true;
    }

    എങ്കിൽ (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_താക്കോൽ_Void_ചിഹ്നം;
        തിരികെ true;
    }

    /* xkb_keysym_from_name needs a C string. */
    അക്ഷരം buf[XKB_KEYSYM_NAME_MAX_SIZE];
    എങ്കിൽ (name.len >= വലുപ്പം(buf)) {
        തിരികെ false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    എങ്കിൽ (sym != XKB_താക്കോൽ_No_ചിഹ്നം) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        തിരികെ true;
    }

    തിരികെ false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { ഘടന scanner *param_scanner }
%parse-param    { ഘടന parser_param *param }

%define parse.error detailed

%token
        END_OF_FILE     0 "end of file"
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
        താക്കോൽ_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        താക്കോലുകൾ_2            30 "keys"
        ROW             31 "row"
        SECTION         32 "section"
        OVERLAY         33 "overlay"
        TEXT            34 "text"
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
        ALPHANUMERIC_താക്കോലുകൾ       73 "alphanumeric_keys"
        MODIFIER_താക്കോലുകൾ           74 "modifier_keys"
        KEYPAD_താക്കോലുകൾ             75 "keypad_keys"
        FUNCTION_താക്കോലുകൾ           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%സംഘം  {
        int64_t          അക്കം;
        എണ്ണൽ xkb_file_type file_type;
        അക്ഷരം            *str;
        ഘടന sval     sval;
        xkb_atom_t      atom;
        എണ്ണൽ merge_mode merge;
        എണ്ണൽ xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        ഘടന { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_ചിഹ്നം_Or_Action_List;
        വ്യഞ്ജകം_Def         *വ്യഞ്ജകം;
        ഘടന { വ്യഞ്ജകം_Def *head; വ്യഞ്ജകം_Def *last; } വ്യഞ്ജകം_List;
        VarDef          *var;
        ഘടന { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        ഘടന { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        താക്കോൽ_Type_Def      *താക്കോൽ_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *താക്കോൽ_Code;
        താക്കോൽ_Alias_Def     *താക്കോൽ_Alias;
        UnknownStatement *unknown;
        ശൂന്യം            *geom;
        XkbFile         *file;
        ഘടന { XkbFile *head; XkbFile *last; } fileList;
}

%തരം <അക്കം>     DECIMAL_DIGIT INTEGER FLOAT
%തരം <str>     STRING
%തരം <sval>    IDENT
%തരം <atom>    KEYNAME
%തരം <അക്കം>     താക്കോൽ_Code_2 സംഖ്യ Integer Float Signed_സംഖ്യ DoodadType
%തരം <merge>   MergeMode OptMergeMode
%തരം <file_type> XkbCompositeType FileType
%തരം <mapFlags> Flag Flags OptFlags
%തരം <str>     MapName OptMapName
%തരം <atom>    FieldSpec Ident Element String
%തരം <keysym>  താക്കോൽ_Sym_2 താക്കോൽ_Sym_Lit
%തരം <no_ചിഹ്നം_Or_Action_List> No_ചിഹ്നം_Or_Action_List
%തരം <any>     Decl
%തരം <anyList> DeclList
%തരം <വ്യഞ്ജകം>    വ്യഞ്ജകം_2 Term Lhs Terminal OptTerminal Coord CoordList
%തരം <വ്യഞ്ജകം>    Multi_താക്കോൽ_Sym_Or_Action_List NonEmptyActions Actions Action
%തരം <വ്യഞ്ജകം>    താക്കോൽ_Syms Non_Empty_താക്കോൽ_Syms താക്കോൽ_Sym_List താക്കോൽ_Or_താക്കോൽ_Sym
%തരം <വ്യഞ്ജകം_List> വ്യഞ്ജകം_List_2 താക്കോൽ_Or_താക്കോൽ_Sym_List
%തരം <വ്യഞ്ജകം_List> ActionList MultiActionList Multi_താക്കോൽ_Sym_List
%തരം <var>     VarDecl SymbolsVarDecl
%തരം <varList> VarDeclList SymbolsBody OptSymbolsBody
%തരം <vmod>    VModDef
%തരം <vmodList> VModDefList VModDecl
%തരം <interp>  InterpretDecl InterpretMatch
%തരം <താക്കോൽ_Type> താക്കോൽ_Type_Decl
%തരം <syms>    SymbolsDecl
%തരം <modMask> ModMapDecl
%തരം <groupCompat> GroupCompatDecl
%തരം <ledMap>  LedMapDecl
%തരം <ledName> LedNameDecl
%തരം <താക്കോൽ_Code> താക്കോൽ_Name_Decl
%തരം <താക്കോൽ_Alias> താക്കോൽ_Alias_Decl
%തരം <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%തരം <geom>    താക്കോലുകൾ_3 താക്കോൽ_4 OverlayDecl Overlay_താക്കോൽ_List Overlay_താക്കോൽ OutlineList OutlineInList
%തരം <geom>    DoodadDecl
%തരം <unknown> UnknownDecl UnknownCompoundStatementDecl
%തരം <file>    XkbFile XkbMapConfig
%തരം <fileList> XkbMapConfigList
%തരം <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <വ്യഞ്ജകം> <var> <vmod> <interp> <താക്കോൽ_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <താക്കോൽ_Code> <താക്കോൽ_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <വ്യഞ്ജകം_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { എങ്കിൽ (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
%destructor { സ്വതന്ത്രം($$); } <str>

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

XkbFile         :       XkbCompositeMap
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (ശൂന്യം) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = ശൂന്യ; param->more_maps = false; }
                ;

XkbCompositeMap :       OptFlags XkbCompositeType OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.head, $1); }
                ;

XkbCompositeType:       XKB_KEYMAP      { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_SEMANTICS   { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_LAYOUT      { $$ = FILE_TYPE_KEYMAP; }
                ;

/*
 * NOTE: Any component is optional
 */
XkbMapConfigList :      XkbMapConfigList XkbMapConfig
                        {
                            എങ്കിൽ ($2) {
                                എങ്കിൽ ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } അല്ലെങ്കിൽ {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ശൂന്യ; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            എങ്കിൽ ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            safe_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.head, $1);
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
                |       ALPHANUMERIC_താക്കോലുകൾ       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_താക്കോലുകൾ           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_താക്കോലുകൾ             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_താക്കോലുകൾ           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            സ്ഥിരം bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (unsigned)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            എങ്കിൽ (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            എങ്കിൽ ($2) {
                                എങ്കിൽ ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } അല്ലെങ്കിൽ {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList OptMergeMode VModDecl
                        {
                            വേണ്ടി (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            എങ്കിൽ ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } അല്ലെങ്കിൽ {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = ശൂന്യ; }
                ;

Decl            :       OptMergeMode VarDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       OptMergeMode InterpretDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode താക്കോൽ_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode താക്കോൽ_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode താക്കോൽ_Type_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode SymbolsDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ModMapDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode GroupCompatDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode LedMapDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode LedNameDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ShapeDecl          { $$ = ശൂന്യ; }
                |       OptMergeMode SectionDecl        { $$ = ശൂന്യ; }
                |       OptMergeMode DoodadDecl         { $$ = ശൂന്യ; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            സ്വതന്ത്രം($2);
                        }
                ;

VarDecl         :       Lhs EQUALS വ്യഞ്ജകം_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

താക്കോൽ_Name_Decl     :       KEYNAME EQUALS താക്കോൽ_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

താക്കോൽ_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = താക്കോൽ_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       VModDef
                        { $$.head = $$.last = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, ശൂന്യ); }
                |       Ident EQUALS വ്യഞ്ജകം_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       താക്കോൽ_Sym_2 PLUS വ്യഞ്ജകം_2
                        { $$ = InterpCreate($1, $3); }
                |       താക്കോൽ_Sym_2
                        { $$ = InterpCreate($1, ശൂന്യ); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            എങ്കിൽ ($2) {
                                എങ്കിൽ ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } അല്ലെങ്കിൽ {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ശൂന്യ; }
                ;

താക്കോൽ_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = താക്കോൽ_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       താക്കോൽ_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = ശൂന്യ; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS വ്യഞ്ജകം_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_താക്കോൽ_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_താക്കോൽ_Sym_Or_Action_List { $$ = VarCreate(ശൂന്യ, $1); }
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
Multi_താക്കോൽ_Sym_Or_Action_List
                :       OBRACKET Multi_താക്കോൽ_Sym_List CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_ചിഹ്നം_Or_Action_List COMMA Multi_താക്കോൽ_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            ഘടന {വ്യഞ്ജകം_Def *head; വ്യഞ്ജകം_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            വേണ്ടി (uint32_t k = 0; k < $2; k++) {
                                വ്യഞ്ജകം_Def* സ്ഥിരം syms =
                                    വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List(XKB_താക്കോൽ_No_ചിഹ്നം);
                                എങ്കിൽ (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.next = &list.head->common;
                                list.head = syms;
                            }
                            $$ = list.head;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_ചിഹ്നം_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            ഘടന {വ്യഞ്ജകം_Def *head; വ്യഞ്ജകം_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            വേണ്ടി (uint32_t k = 0; k < $2; k++) {
                                വ്യഞ്ജകം_Def* സ്ഥിരം acts = വ്യഞ്ജകം_Create_Action_List(ശൂന്യ);
                                എങ്കിൽ (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.next = &list.head->common;
                                list.head = acts;
                            }
                            $$ = list.head;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_ചിഹ്നം_Or_Action_List CBRACKET
                        { $$ = വ്യഞ്ജകം_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_ചിഹ്നം_Or_Action_List
                :       No_ചിഹ്നം_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS വ്യഞ്ജകം_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP വ്യഞ്ജകം_2 OBRACE താക്കോൽ_Or_താക്കോൽ_Sym_List CBRACE SEMI
                        {
                            എങ്കിൽ (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.തരം != STMT_വ്യഞ്ജകം_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.head);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.head);
                        }
                ;

താക്കോൽ_Or_താക്കോൽ_Sym_List :       താക്കോൽ_Or_താക്കോൽ_Sym_List COMMA താക്കോൽ_Or_താക്കോൽ_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       താക്കോൽ_Or_താക്കോൽ_Sym
                        { $$.head = $$.last = $1; }
                ;

താക്കോൽ_Or_താക്കോൽ_Sym     :       KEYNAME
                        { $$ = വ്യഞ്ജകം_Create_താക്കോൽ_Name($1); }
                |       താക്കോൽ_Sym_2
                        { $$ = വ്യഞ്ജകം_Create_താക്കോൽ_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS വ്യഞ്ജകം_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS വ്യഞ്ജകം_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS വ്യഞ്ജകം_2 SEMI
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
                            FreeStmt((ParseCommon *) $4.head);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = ശൂന്യ; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (ശൂന്യം) $4; $$ = ശൂന്യ; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ശൂന്യ; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = ശൂന്യ;}
                |       SectionBodyItem                 { $$ = ശൂന്യ; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = ശൂന്യ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ശൂന്യ; }
                |       DoodadDecl
                        { $$ = ശൂന്യ; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ശൂന്യ; }
                |       OverlayDecl
                        { $$ = ശൂന്യ; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = ശൂന്യ;}
                |       RowBodyItem             { $$ = ശൂന്യ; }
                ;

RowBodyItem     :       താക്കോലുകൾ_2 OBRACE താക്കോലുകൾ_3 CBRACE SEMI { $$ = ശൂന്യ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ശൂന്യ; }
                ;

താക്കോലുകൾ_3            :       താക്കോലുകൾ_3 COMMA താക്കോൽ_4          { $$ = ശൂന്യ; }
                |       താക്കോൽ_4                     { $$ = ശൂന്യ; }
                ;

താക്കോൽ_4             :       KEYNAME
                        { $$ = ശൂന്യ; }
                |       OBRACE വ്യഞ്ജകം_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = ശൂന്യ; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_താക്കോൽ_List CBRACE SEMI
                        { $$ = ശൂന്യ; }
                ;

Overlay_താക്കോൽ_List  :       Overlay_താക്കോൽ_List COMMA Overlay_താക്കോൽ { $$ = ശൂന്യ; }
                |       Overlay_താക്കോൽ                      { $$ = ശൂന്യ; }
                ;

Overlay_താക്കോൽ      :       KEYNAME EQUALS KEYNAME          { $$ = ശൂന്യ; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = ശൂന്യ;}
                |       OutlineInList
                        { $$ = ശൂന്യ; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (ശൂന്യം) $2; $$ = ശൂന്യ; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (ശൂന്യം) $4; $$ = ശൂന്യ; }
                |       Ident EQUALS വ്യഞ്ജകം_2
                        { FreeStmt((ParseCommon *) $3); $$ = ശൂന്യ; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (ശൂന്യം) $1; (ശൂന്യം) $3; $$ = ശൂന്യ; }
                |       Coord
                        { (ശൂന്യം) $1; $$ = ശൂന്യ; }
                ;

Coord           :       OBRACKET Signed_സംഖ്യ COMMA Signed_സംഖ്യ CBRACKET
                        { $$ = ശൂന്യ; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = ശൂന്യ; }
                ;

DoodadType      :       TEXT    { $$ = 0; }
                |       OUTLINE { $$ = 0; }
                |       SOLID   { $$ = 0; }
                |       LOGO    { $$ = 0; }
                ;

FieldSpec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       താക്കോൽ_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       GROUP
                        { $$ = xkb_atom_intern_literal(param->ctx, "group"); }
                |       MODIFIER_MAP
                        {$$ = xkb_atom_intern_literal(param->ctx, "modifier_map");}
                |       INDICATOR
                        { $$ = xkb_atom_intern_literal(param->ctx, "indicator"); }
                |       SHAPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "shape"); }
                |       ROW
                        { $$ = xkb_atom_intern_literal(param->ctx, "row"); }
                |       SECTION
                        { $$ = xkb_atom_intern_literal(param->ctx, "section"); }
                |       TEXT
                        { $$ = xkb_atom_intern_literal(param->ctx, "text"); }
                ;

OptMergeMode    :       MergeMode       { $$ = $1; }
                |                       { $$ = MERGE_DEFAULT; }
                ;

MergeMode       :       INCLUDE         { $$ = MERGE_DEFAULT; }
                |       AUGMENT         { $$ = MERGE_AUGMENT; }
                |       OVERRIDE        { $$ = MERGE_OVERRIDE; }
                |       REPLACE         { $$ = MERGE_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_MESSAGE_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = MERGE_DEFAULT;
                }
                ;

വ്യഞ്ജകം_List_2        :       വ്യഞ്ജകം_List_2 COMMA വ്യഞ്ജകം_2
                        {
                            എങ്കിൽ ($3) {
                                എങ്കിൽ ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } അല്ലെങ്കിൽ {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       വ്യഞ്ജകം_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = ശൂന്യ; }
                ;

വ്യഞ്ജകം_2            :       വ്യഞ്ജകം_2 DIVIDE വ്യഞ്ജകം_2
                        { $$ = വ്യഞ്ജകം_Create_ദ്വിമാന(STMT_വ്യഞ്ജകം_DIVIDE, $1, $3); }
                |       വ്യഞ്ജകം_2 PLUS വ്യഞ്ജകം_2
                        { $$ = വ്യഞ്ജകം_Create_ദ്വിമാന(STMT_വ്യഞ്ജകം_ADD, $1, $3); }
                |       വ്യഞ്ജകം_2 MINUS വ്യഞ്ജകം_2
                        { $$ = വ്യഞ്ജകം_Create_ദ്വിമാന(STMT_വ്യഞ്ജകം_SUBTRACT, $1, $3); }
                |       വ്യഞ്ജകം_2 TIMES വ്യഞ്ജകം_2
                        { $$ = വ്യഞ്ജകം_Create_ദ്വിമാന(STMT_വ്യഞ്ജകം_MULTIPLY, $1, $3); }
                |       Lhs EQUALS വ്യഞ്ജകം_2
                        { $$ = വ്യഞ്ജകം_Create_ദ്വിമാന(STMT_വ്യഞ്ജകം_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = വ്യഞ്ജകം_Create_Unary(STMT_വ്യഞ്ജകം_NEGATE, $2); }
                |       PLUS Term
                        { $$ = വ്യഞ്ജകം_Create_Unary(STMT_വ്യഞ്ജകം_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = വ്യഞ്ജകം_Create_Unary(STMT_വ്യഞ്ജകം_NOT, $2); }
                |       INVERT Term
                        { $$ = വ്യഞ്ജകം_Create_Unary(STMT_വ്യഞ്ജകം_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN വ്യഞ്ജകം_List_2 CPAREN %prec OPAREN
                        { $$ = വ്യഞ്ജകം_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN വ്യഞ്ജകം_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &വ്യഞ്ജകം->common; $$.last = വ്യഞ്ജകം;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = വ്യഞ്ജകം_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = വ്യഞ്ജകം_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = വ്യഞ്ജകം_Create_Action_List(ശൂന്യ); }
                ;

Action          :       FieldSpec OPAREN വ്യഞ്ജകം_List_2 CPAREN
                        { $$ = വ്യഞ്ജകം_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = വ്യഞ്ജകം_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = വ്യഞ്ജകം_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET വ്യഞ്ജകം_2 CBRACKET
                        { $$ = വ്യഞ്ജകം_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET വ്യഞ്ജകം_2 CBRACKET
                        { $$ = വ്യഞ്ജകം_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ശൂന്യ; }
                ;

Terminal        :       String
                        { $$ = വ്യഞ്ജകം_Create_String($1); }
                |       Integer
                        { $$ = വ്യഞ്ജകം_Create_Integer($1); }
                |       Float
                        { $$ = വ്യഞ്ജകം_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = വ്യഞ്ജകം_Create_താക്കോൽ_Name($1); }
                ;

Multi_താക്കോൽ_Sym_List :       Multi_താക്കോൽ_Sym_List COMMA താക്കോൽ_Sym_Lit
                        {
                            വ്യഞ്ജകം_Def *വ്യഞ്ജകം = വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &വ്യഞ്ജകം->common; $$.last = വ്യഞ്ജകം;
                        }
                |       Multi_താക്കോൽ_Sym_List COMMA താക്കോൽ_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       താക്കോൽ_Sym_Lit
                        { $$.head = $$.last = വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List($1); }
                |       Non_Empty_താക്കോൽ_Syms
                        { $$.head = $$.last = $1; }
                ;

താക്കോൽ_Sym_List      :       താക്കോൽ_Sym_List COMMA താക്കോൽ_Sym_Lit
                        { $$ = വ്യഞ്ജകം_Append_താക്കോൽ_Sym_List($1, $3); }
                |       താക്കോൽ_Sym_List COMMA STRING
                        {
                            $$ = വ്യഞ്ജകം_താക്കോൽ_Sym_List_Append_String(param->scanner, $1, $3);
                            സ്വതന്ത്രം($3);
                            എങ്കിൽ (!$$)
                                YYERROR;
                        }
                |       താക്കോൽ_Sym_Lit
                        {
                            $$ = വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List($1);
                            എങ്കിൽ (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List(XKB_താക്കോൽ_No_ചിഹ്നം);
                            എങ്കിൽ (!$$)
                                YYERROR;
                            $$ = വ്യഞ്ജകം_താക്കോൽ_Sym_List_Append_String(param->scanner, $$, $1);
                            സ്വതന്ത്രം($1);
                            എങ്കിൽ (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_താക്കോൽ_Syms :       OBRACE താക്കോൽ_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List(XKB_താക്കോൽ_No_ചിഹ്നം);
                            എങ്കിൽ (!$$)
                                YYERROR;
                            $$ = വ്യഞ്ജകം_താക്കോൽ_Sym_List_Append_String(param->scanner, $$, $1);
                            സ്വതന്ത്രം($1);
                            എങ്കിൽ (!$$)
                                YYERROR;
                        }
                ;

താക്കോൽ_Syms         :       Non_Empty_താക്കോൽ_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = വ്യഞ്ജകം_Create_താക്കോൽ_Sym_List(XKB_താക്കോൽ_No_ചിഹ്നം); }
                ;

താക്കോൽ_Sym_2          :       താക്കോൽ_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            സ്വതന്ത്രം($1);
                            എങ്കിൽ ($$ == XKB_താക്കോൽ_No_ചിഹ്നം)
                                YYERROR;
                        }
                ;

താക്കോൽ_Sym_Lit       :       IDENT
                        {
                            എങ്കിൽ (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned പൂർണ്ണസംഖ്യ) $1.len, $1.start
                                );
                                $$ = XKB_താക്കോൽ_No_ചിഹ്നം;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_താക്കോൽ_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_താക്കോൽ_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            എങ്കിൽ ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_താക്കോൽ_No_ചിഹ്നം;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            അല്ലെങ്കിൽ {
                                /* Any other numeric value */
                                എങ്കിൽ ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ശൂന്യ, $$, "%#06"PRIx32, "");
                                } അല്ലെങ്കിൽ {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_താക്കോൽ_No_ചിഹ്നം;
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

Signed_സംഖ്യ    :       MINUS സംഖ്യ    { $$ = -$2; }
                |       സംഖ്യ          { $$ = $1; }
                ;

സംഖ്യ          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

താക്കോൽ_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); സ്വതന്ത്രം($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ശൂന്യ; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(ഘടന xkb_context *ctx, സ്ഥിരം ഘടന parser_keymap_config *config,
      ഘടന scanner *scanner, സ്ഥിരം അക്ഷരം *map)
{
    പൂർണ്ണസംഖ്യ ret;
    XkbFile *first = ശൂന്യ;
    ഘടന parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ശൂന്യ,
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

    ആകുമ്പോൾ ((ret = yyparse(&param)) == 0 && param.more_maps) {
        എങ്കിൽ (map) {
            എങ്കിൽ (streq_not_null(map, param.rtrn->name))
                തിരികെ param.rtrn;
            അല്ലെങ്കിൽ
                FreeXkbFile(param.rtrn);
        }
        അല്ലെങ്കിൽ {
            എങ്കിൽ (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                തിരികെ param.rtrn;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (!first) {
                first = param.rtrn;
            }
            അല്ലെങ്കിൽ {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ശൂന്യ;
    }

    എങ്കിൽ (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        തിരികെ ശൂന്യ;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    എങ്കിൽ (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    തിരികെ first;
}

/* Parse the next section */
bool
parse_next(ഘടന xkb_context *ctx,
           സ്ഥിരം ഘടന parser_keymap_config *config,
           ഘടന scanner *scanner, XkbFile **xkb_file)
{
    പൂർണ്ണസംഖ്യ ret;
    ഘടന parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ശൂന്യ,
        .config = *config,
        .more_maps = false,
    };

    എങ്കിൽ ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        തിരികെ true;
    } അല്ലെങ്കിൽ {
        FreeXkbFile(param.rtrn);
        *xkb_file = ശൂന്യ;
        തിരികെ (ret == 0);
    }
}
